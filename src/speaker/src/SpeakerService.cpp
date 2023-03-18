#include "speaker/SpeakerService.hpp"

#include "dbus/Config.hpp"
#include "speaker/ISpeaker.hpp"
#include "speaker/SpeakerAdapter.hpp"

#include <boost/assert.hpp>
#include <sdbus-c++/AdaptorInterfaces.h>
#include <sdbus-c++/IConnection.h>

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

class SpeakerService::Impl final : public sdbus::AdaptorInterfaces<SpeakerAdapter> {
public:
    explicit Impl(ISpeaker& speaker)
        : AdaptorInterfaces{getConnection(), dbus::kObjectPath}
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
    : _impl{std::make_unique<Impl>(speaker)}
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
