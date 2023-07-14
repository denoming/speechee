#pragma once

#include "speaker/AvailabilityObserver.hpp"
#include "speaker/MqttPublisher.hpp"
#include "speaker/Types.hpp"

#include <sigc++/trackable.h>

namespace jar {

class AvailabilityPublisher : public sigc::trackable {
public:
    explicit AvailabilityPublisher(MqttPublisher& publisher, AvailabilityObserver& observer);

private:
    void
    publish();

    void
    onAvailabilityUpdate(const std::string& name, AvailabilityState state);

    void
    onPublisherConnect(bool success, MqttConnectReturnCode code);

private:
    MqttPublisher& _publisher;
    std::string _name;
    AvailabilityState _state{AvailabilityState::Offline};
};

} // namespace jar