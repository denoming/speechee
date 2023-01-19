#include "speaker/SpeakerService.hpp"

#include "dbus/DbusConfig.hpp"
#include "jarvis-speaker-server-glue.h"
#include "speaker/ISpeaker.hpp"

#include <boost/assert.hpp>
#include <sdbus-c++/sdbus-c++.h>

namespace jar {

static sdbus::IConnection&
getConnection()
{
    static std::unique_ptr<sdbus::IConnection> connection;
    if (!connection) {
        connection = sdbus::createSystemBusConnection(std::string{dbus::kServiceName});
        BOOST_ASSERT(connection);
        connection->enterEventLoopAsync();
    }
    return *connection;
}

class SpeakerService::Impl final
    : public sdbus::AdaptorInterfaces<org::denoming::jarvis::speaker_adaptor> {
public:
    Impl(sdbus::IConnection& connection, std::string objectPath, ISpeaker& speaker)
        : AdaptorInterfaces{connection, std::move(objectPath)}
        , _speaker{speaker}
    {
    }

    void
    synthesizeText(const std::string& text, const std::string& lang) final
    {
        _speaker.synthesizeText(text, lang);
    }

    void
    synthesizeSsml(const std::string& ssml, const std::string& lang) final
    {
        _speaker.synthesizeSsml(ssml, lang);
    }

private:
    ISpeaker& _speaker;
};

SpeakerService::SpeakerService(ISpeaker& speaker)
    : _impl{std::make_unique<Impl>(getConnection(), std::string{dbus::kObjectPath}, speaker)}
{
}

SpeakerService::~SpeakerService() = default;

void
SpeakerService::start()
{
    BOOST_ASSERT(_impl);
    _impl->registerAdaptor();
}

void
SpeakerService::stop()
{
    BOOST_ASSERT(_impl);
    _impl->unregisterAdaptor();
}

} // namespace jar
