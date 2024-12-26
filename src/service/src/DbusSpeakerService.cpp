#include "speechee/DbusSpeakerService.hpp"

#include <jarvisto/core/Logger.hpp>

#include "dbus/Config.hpp"
#include "speechee/ISpeaker.hpp"
#include "speechee/SpeakerAdapter.hpp"

#include <sdbus-c++/AdaptorInterfaces.h>
#include <sdbus-c++/IConnection.h>

#include <gsl/gsl-lite.hpp>

namespace jar {

static sdbus::IConnection&
getConnection()
{
    static std::unique_ptr<sdbus::IConnection> connection;
    if (not connection) {
        connection = sdbus::createSystemBusConnection(dbus::kServiceName);
        gsl_Assert(connection);
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
        gsl_Assert(_impl);
        if (_impl) {
            _impl->registerAdaptor();
            availability(AvailabilityState::Online);
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
            availability(AvailabilityState::Offline);
            _impl->unregisterAdaptor();
        }
    } catch (const std::exception& e) {
        LOGE("Exception: {}", e.what());
    }
}

} // namespace jar
