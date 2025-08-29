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

#include "speechee/SpeakerAdapter.hpp"

#include "dbus/Config.hpp"

#include <sdbus-c++/IObject.h>

namespace jar {

SpeakerAdapter::SpeakerAdapter(sdbus::IObject& object)
    : _object{object}
{
}

void
SpeakerAdapter::registerAdaptor()
{
    _object
        .addVTable(sdbus::registerMethod(dbus::client::SynthesizeText.name)
                       .implementedAs([this](const std::string& text, const std::string& lang) {
                           synthesizeText(text, lang);
                       })
                       .withNoReply(),
                   sdbus::registerMethod(dbus::client::SynthesizeSsml.name)
                       .implementedAs([this](const std::string& ssml, const std::string& lang) {
                           synthesizeSsml(ssml, lang);
                       })
                       .withNoReply())
        .forInterface(dbus::client::SynthesizeText.interface);
}

} // namespace jar