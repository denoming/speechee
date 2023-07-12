#include "speaker/DbusSpeakerService.hpp"

#include <jarvisto/Logger.hpp>

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

class DbusSpeakerService::Impl final : public sdbus::AdaptorInterfaces<SpeakerAdapter> {
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

DbusSpeakerService::DbusSpeakerService(ISpeaker& speaker)
    : AvailabilitySubject{"SpeakerService.DBus"}
{
    try {
        _impl = std::make_unique<Impl>(speaker);
    } catch (const std::exception& e) {
        LOGE("Exception: {}", e.what());
    }
}

DbusSpeakerService::~DbusSpeakerService()
{
    if (_impl) {
        _impl.reset();
    }
}

bool
DbusSpeakerService::start()
{
    try {
        BOOST_ASSERT(_impl);
        if (_impl) {
            _impl->registerAdaptor();
            availability(AvailabilityStatus::Online);
            return true;
        }
    } catch (const std::exception& e) {
        LOGE("Exception: {}", e.what());
    }
    return false;
}

void
DbusSpeakerService::stop()
{
    try {
        if (_impl) {
            availability(AvailabilityStatus::Offline);
            _impl->unregisterAdaptor();
        }
    } catch (const std::exception& e) {
        LOGE("Exception: {}", e.what());
    }
}

} // namespace jar
