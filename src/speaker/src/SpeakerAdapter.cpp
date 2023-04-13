#include "speaker/SpeakerAdapter.hpp"

#include "dbus/Config.hpp"

#include <sdbus-c++/IObject.h>

namespace jar {

SpeakerAdapter::SpeakerAdapter(sdbus::IObject& object)
{
    object.registerMethod(dbus::client::SynthesizeText.name)
        .onInterface(dbus::client::SynthesizeText.interface)
        .implementedAs([this](const std::string& text, const std::string& lang) {
            synthesizeText(text, lang);
        })
        .withNoReply();

    object.registerMethod(dbus::client::SynthesizeSsml.name)
        .onInterface(dbus::client::SynthesizeSsml.interface)
        .implementedAs([this](const std::string& ssml, const std::string& lang) {
            synthesizeSsml(ssml, lang);
        })
        .withNoReply();
}

} // namespace jar