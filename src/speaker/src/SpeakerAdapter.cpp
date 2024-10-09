#include "speaker/SpeakerAdapter.hpp"

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