#include "speaker/Types.hpp"

#include "speaker/Formatters.hpp"

#include <fmt/format.h>

namespace jar {

std::ostream&
operator<<(std::ostream& s, PlayState in)
{
    s << fmt::to_string(in);
    return s;
}

} // namespace jar
