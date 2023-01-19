#include "ServiceApplication.hpp"

#include "jarvis/Logger.hpp"
#include "speaker/SpeakerSubsystem.hpp"

namespace jar {

const char*
ServiceApplication::name() const
{
    return "Speaker";
}

void
ServiceApplication::initialize(Application& application)
{
    addSubsystem(std::make_unique<SpeakerSubsystem>());

    Application::initialize(application);
}

void
ServiceApplication::proceed()
{
    if (!waitForTermination()) {
        LOGE("Waiting for termination has failed");
    }
}

} // namespace jar

APP_MAIN(jar::ServiceApplication)
