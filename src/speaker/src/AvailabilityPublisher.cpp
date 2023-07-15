#include "speaker/AvailabilityPublisher.hpp"

#include "speaker/Formatters.hpp"
#include "speechee/Options.hpp"

#include <fmt/format.h>
#include <jarvisto/Logger.hpp>
#include <sigc++/adaptors/track_obj.h>

#ifdef ENABLE_INTEGRATION
#include <boost/json.hpp>
namespace json = boost::json;
#endif

namespace {

#ifdef ENABLE_INTEGRATION
const std::string&
getIntegrationTopic()
{
    static const std::string kTopic{"homeassistant/sensor/speechee/speaker/config"};
    return kTopic;
}

const std::string&
getIntegrationMessage()
{
    static const std::string kMessage = json::serialize(json::value{
        {
            "availability",
            json::array{{{"topic", "speechee/speaker/state"},
                         {"payload_available", "Online"},
                         {"payload_not_available", "Offline"}}},
        },
        {"entity_category", "diagnostic"},
        {"name", "speechee/speaker"},
        {"state_topic", "speechee/speaker/state"},
    });
    return kMessage;
}
#endif

} // namespace

namespace jar {

AvailabilityPublisher::AvailabilityPublisher(MqttPublisher& publisher,
                                             AvailabilityObserver& observer)
    : _publisher{publisher}
    , _name{observer.name()}
    , _state{observer.state()}
{

    observer.onStateUpdate(
        sigc::track_obj([this](auto&& name, auto&& state) { onStateUpdate(name, state); }, this));
    publisher.onConnect(sigc::track_obj(
        [this](auto&& success, auto&& code) { onPublisherConnect(success, code); }, this));

    if (publisher.connected()) {
        publish();
    }
}

void
AvailabilityPublisher::publish()
{
#ifdef ENABLE_INTEGRATION
    /* Publish integration MQTT discovery message */
    _publisher.publish(getIntegrationTopic(), getIntegrationMessage());
#endif

    static const std::string kPublishTopic = fmt::format("{}/speaker/state", _name);
    if (!_publisher.publish(kPublishTopic, fmt::to_string(_state))) {
        LOGE("Unable to publish <{}> state", _state);
    }
}

void
AvailabilityPublisher::onStateUpdate(const std::string& name, const AvailabilityState state)
{
    LOGD("State of <{}> subject update: old<{}>, new<{}>", name, _state, state);

    _state = state;

    publish();
}

void
AvailabilityPublisher::onPublisherConnect(const bool success, const MqttConnectReturnCode code)
{
    LOGD("Connection state update: success<{}>, code<{}>", success, code);

    if (success) {
        publish();
    }
}

} // namespace jar