#include "jarvis/SpeakerClient.hpp"

#include "dbus/DbusConfig.hpp"
#include "client/SpeakerClientImpl.hpp"

namespace jar {

SpeakerClient::SpeakerClient()
    : _impl{std::make_unique<SpeakerClientImpl>(std::string{dbus::kServiceName},
                                                std::string{dbus::kObjectPath})}
{
}

SpeakerClient::~SpeakerClient() = default;

void
SpeakerClient::synthesizeText(const std::string& text, const std::string& lang)
{
    _impl->synthesizeText(text, lang);
}

void
SpeakerClient::synthesizeSsml(const std::string& ssml, const std::string& lang)
{
    _impl->synthesizeSsml(ssml, lang);
}

} // namespace jar
