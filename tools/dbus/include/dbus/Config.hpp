#pragma once

#include <string_view>

namespace jar::dbus {

struct MethodSpec {
    const char* interface = nullptr;
    const char* name = nullptr;
};

struct SignalSpec {
    const char* interface = nullptr;
    const char* name = nullptr;
};

/* The dbus service name */
inline const char* kServiceName{"org.denoming.speechee"};
/* The object path to dbus service */
inline const char* kObjectPath{"/org/denoming/speechee"};
/* The interface name */
inline const char* kInterfaceName{"org.denoming.speechee"};

namespace client {
// clang-format off
inline const auto SynthesizeText
    = MethodSpec{kInterfaceName, "SynthesizeText"};
inline const auto SynthesizeSsml
    = MethodSpec{kInterfaceName, "SynthesizeSsml"};
//clang-format on
}

} // namespace jar::dbus