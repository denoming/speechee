#include "ServiceApplication.hpp"

#include "speaker/SpeakerSubsystem.hpp"
#include "speechee/Options.hpp"

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
#ifdef ENABLE_DLT
    LoggerInitializer::instance().initialize("SPCH", name(), "MAIN", "Main Context");
#else
    LoggerInitializer::instance().initialize();
#endif

    addSubsystem(std::make_unique<SpeakerSubsystem>());

    Application::initialize(application);
}

} // namespace jar

APP_MAIN(jar::ServiceApplication)
