#pragma once

#include "speaker/Types.hpp"

#include <sigc++/trackable.h>

namespace jar {

class MqttBasicClient;
class AvailabilityObserver;

class AvailabilityPublisher : public sigc::trackable {
public:
    explicit AvailabilityPublisher(MqttBasicClient& client, AvailabilityObserver& observer);

private:
    void
    publish();

    void
    onStateUpdate(const std::string& name, AvailabilityState state);

    void
    onPublisherConnect(bool hasConnection);

private:
    MqttBasicClient& _client;
    std::string _name;
    AvailabilityState _state{AvailabilityState::Offline};
};

} // namespace jar