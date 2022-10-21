#pragma once

#include <string_view>

namespace jar::dbus {

/* The dbus service name */
inline constexpr std::string_view kServiceName{"org.denoming.jarvis.speaker"};
/* The object path to dbus service */
inline constexpr std::string_view kObjectPath{"/org/denoming/jarvis/speaker"};

} // namespace jar::dbus