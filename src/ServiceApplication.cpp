#include "ServiceApplication.hpp"

#include "speaker/SpeakerSubsystem.hpp"

#include <jarvisto/LoggerInitializer.hpp>

namespace jar {

const char*
ServiceApplication::name() const
{
    return "Speechee";
}

void
ServiceApplication::initialize(Application& application)
{
    LoggerInitializer::instance().initialize("SPCH", name(), "MAIN", "Main Context");

    addSubsystem(std::make_unique<SpeakerSubsystem>());

    Application::initialize(application);
}

} // namespace jar

APP_MAIN(jar::ServiceApplication)
