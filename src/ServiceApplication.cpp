#include "ServiceApplication.hpp"

#include "speechee/SpeakerSubsystem.hpp"
#include "speechee/ServiceLogger.hpp"

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
    ServiceLogger logger{"SPEECHEE"};
    logger.create("MAIN", SPDLOG_LEVEL_DEBUG);

    addSubsystem(std::make_unique<SpeakerSubsystem>());

    Application::initialize(application);
}

} // namespace jar

APP_MAIN(jar::ServiceApplication)
