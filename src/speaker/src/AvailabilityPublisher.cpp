#include "speaker/AvailabilityPublisher.hpp"

#include "speaker/Formatters.hpp"

#include <fmt/format.h>
#include <jarvisto/Logger.hpp>
#include <sigc++/adaptors/track_obj.h>

namespace jar {

AvailabilityPublisher::AvailabilityPublisher(MqttPublisher& publisher,
                                             AvailabilityObserver& observer)
    : _publisher{publisher}
    , _name{observer.name()}
    , _state{observer.state()}
{

    observer.onStateUpdate(sigc::track_obj(
        [this](auto&& name, auto&& state) { onAvailabilityUpdate(name, state); }, this));
    publisher.onConnect(sigc::track_obj(
        [this](auto&& success, auto&& code) { onPublisherConnect(success, code); }, this));

    publish();
}

void
AvailabilityPublisher::publish()
{
    if (_publisher.connected()) {
        static const std::string topic = fmt::format("{}/speaker/state", _name);
        if (!_publisher.publish(topic, fmt::to_string(_state))) {
            LOGE("Unable to publish <{}> state", _state);
        }
    }
}

void
AvailabilityPublisher::onAvailabilityUpdate(const std::string& /*name*/,
                                            const AvailabilityState state)
{
    _state = state;

    publish();
}

void
AvailabilityPublisher::onPublisherConnect(bool success, MqttConnectReturnCode /*code*/)
{
    if (success) {
        publish();
    }
}

} // namespace jar