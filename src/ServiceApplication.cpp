#include "ServiceApplication.hpp"

#include "speechee/SpeakerSubsystem.hpp"
#include "speechee/Options.hpp"

#include <jarvisto/core/LoggerInitializer.hpp>
#include <jarvisto/core/SignalHandler.hpp>

namespace jar {

const char*
ServiceApplication::name() const
{
    return "Speechee";
}

void
ServiceApplication::proceed()
{
    const SignalHandler handler{SIGINT, SIGTERM};
    std::ignore = handler.wait();
}

void
ServiceApplication::initialize(Application& application)
{
    LoggerInitializer::instance().initialize();

    addSubsystem(std::make_unique<SpeakerSubsystem>());

    Application::initialize(application);
}

} // namespace jar

APP_MAIN(jar::ServiceApplication)
