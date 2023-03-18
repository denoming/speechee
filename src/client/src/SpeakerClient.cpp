#include "jarvis/SpeakerClient.hpp"

#include "client/SpeakerProxy.hpp"
#include "dbus/Config.hpp"

#include <boost/assert.hpp>
#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>

namespace jar {

class SpeakerClient::Impl final : public sdbus::ProxyInterfaces<SpeakerProxy> {
public:
    Impl()
        : sdbus::ProxyInterfaces<SpeakerProxy>(dbus::kServiceName, dbus::kObjectPath)
    {
        registerProxy();
    }

    ~Impl() final
    {
        unregisterProxy();
    }

    void
    synthesizeText(const std::string& text, const std::string& lang)
    {
        getProxy()
            .callMethod(dbus::client::SynthesizeText.name)
            .onInterface(dbus::client::SynthesizeText.interface)
            .withArguments(text, lang)
            .dontExpectReply();
    }

    void
    synthesizeSsml(const std::string& text, const std::string& lang)
    {
        getProxy()
            .callMethod(dbus::client::SynthesizeSsml.name)
            .onInterface(dbus::client::SynthesizeSsml.interface)
            .withArguments(text, lang)
            .dontExpectReply();
    }
};

SpeakerClient::SpeakerClient()
    : _impl{std::make_unique<Impl>()}
{
}

SpeakerClient::~SpeakerClient() = default;

void
SpeakerClient::synthesizeText(const std::string& text, const std::string& lang)
{
    BOOST_ASSERT(_impl);
    _impl->synthesizeText(text, lang);
}

void
SpeakerClient::synthesizeSsml(const std::string& ssml, const std::string& lang)
{
    BOOST_ASSERT(_impl);
    _impl->synthesizeSsml(ssml, lang);
}

} // namespace jar
