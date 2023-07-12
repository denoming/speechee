#pragma once

#include <ostream>

namespace jar {

enum class PlayState { Null, Idle, Busy, Error };

enum class AvailabilityStatus { Online, Offline };

std::ostream& operator<<(std::ostream& s, PlayState in);
std::ostream& operator<<(std::ostream& s, AvailabilityStatus in);

} // namespace jar
