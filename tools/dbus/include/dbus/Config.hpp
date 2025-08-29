// Copyright 2025 Denys Asauliak
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <sdbus-c++/Types.h>

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
inline const sdbus::ServiceName kServiceName{"org.denoming.speechee"};
/* The object path to dbus service */
inline const sdbus::ObjectPath kObjectPath{"/org/denoming/speechee"};
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