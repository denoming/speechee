#include "SpeakerApplication.hpp"

#include "jarvis/Logger.hpp"
#include "speaker/SpeakerSubsystem.hpp"

namespace jar {

const char*
SpeakerApplication::name() const
{
    return "Speaker";
}

void
SpeakerApplication::initialize(Application& application)
{
    addSubsystem(std::make_unique<SpeakerSubsystem>());

    Application::initialize(application);
}

void
SpeakerApplication::proceed()
{
    if (!waitForTermination()) {
        LOGE("Waiting for termination has failed");
    }
}

} // namespace jar

APP_MAIN(jar::SpeakerApplication)
