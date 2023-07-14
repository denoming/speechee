#pragma once

#include <ostream>

namespace jar {

enum class PlayState { Null, Idle, Busy, Error };

enum class AvailabilityState { Online, Offline };

enum class MqttConnectReturnCode {
    Accepted = 0,
    UnacceptableProtocolVersion,
    IdentifierRejected,
    ServerUnavailable,
    BadCredentials,
    NotAuthorized
};

std::ostream& operator<<(std::ostream& s, PlayState in);
std::ostream& operator<<(std::ostream& s, AvailabilityState in);

} // namespace jar
