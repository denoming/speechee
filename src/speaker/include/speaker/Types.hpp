#pragma once

#include <ostream>

namespace jar {

enum class PlayState { Null, Idle, Busy, Error };

std::ostream& operator<<(std::ostream& s, PlayState in);

} // namespace jar
