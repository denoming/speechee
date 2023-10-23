#include "speaker/AvailabilityPublisher.hpp"

#include "speaker/AvailabilityObserver.hpp"
#include "speaker/Formatters.hpp"
#include "speechee/Options.hpp"

#include <fmt/format.h>
#include <jarvisto/Logger.hpp>
#include <jarvisto/MqttBasicClient.hpp>
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

AvailabilityPublisher::AvailabilityPublisher(MqttBasicClient& client,
                                             AvailabilityObserver& observer)
    : _client{client}
    , _name{observer.name()}
    , _state{observer.state()}
{

    observer.onStateUpdate(
        sigc::track_obj([this](auto&& name, auto&& state) { onStateUpdate(name, state); }, this));

    std::ignore = _client.onConnect(sigc::track_obj(
        [this](const MqttReturnCode code) { onPublisherConnect(code == MqttReturnCode::Accepted); },
        this));

    if (_client.hasConnection()) {
        publish();
    }
}

void
AvailabilityPublisher::publish()
{
#ifdef ENABLE_INTEGRATION
    /* Publish integration MQTT discovery message */
    if (auto rv = _client.publish(getIntegrationTopic(), getIntegrationMessage()); not rv) {
        LOGE("Unable to publish integration discovery message: {}", rv.error().message());
    }
#endif

    static const std::string kPublishTopic = fmt::format("{}/speaker/state", _name);
    if (auto rv = _client.publish(kPublishTopic, fmt::to_string(_state)); not rv) {
        LOGE("Unable to publish <{}> state: {}", _state, rv.error().message());
    }
}

void
AvailabilityPublisher::onStateUpdate(const std::string& name, const AvailabilityState state)
{
    LOGD("State of <{}> subject update: old<{}>, new<{}>", name, _state, state);

    _state = state;

    if (_client.hasConnection()) {
        publish();
    }
}

void
AvailabilityPublisher::onPublisherConnect(const bool hasConnection)
{
    LOGD("Connection state update: hasConnection<{}>", hasConnection);

    if (hasConnection) {
        publish();
    }
}

} // namespace jar