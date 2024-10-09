#include "speechee/DbusSpeakerClient.hpp"

#include "client/DbusSpeakerProxy.hpp"
#include "dbus/Config.hpp"

#include <boost/assert.hpp>
#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>

namespace jar {

class DbusSpeakerClient::Impl final : public sdbus::ProxyInterfaces<DbusSpeakerProxy> {
public:
    Impl()
        : sdbus::ProxyInterfaces<DbusSpeakerProxy>(dbus::kServiceName, dbus::kObjectPath)
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

DbusSpeakerClient::DbusSpeakerClient()
    : _impl{std::make_unique<Impl>()}
{
}

DbusSpeakerClient::~DbusSpeakerClient() = default;

void
DbusSpeakerClient::synthesizeText(const std::string& text, const std::string& lang)
{
    BOOST_ASSERT(_impl);
    _impl->synthesizeText(text, lang);
}

void
DbusSpeakerClient::synthesizeSsml(const std::string& ssml, const std::string& lang)
{
    BOOST_ASSERT(_impl);
    _impl->synthesizeSsml(ssml, lang);
}

} // namespace jar
