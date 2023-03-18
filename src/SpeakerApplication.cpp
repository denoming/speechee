#include "SpeakerApplication.hpp"

#include "speaker/SpeakerSubsystem.hpp"

namespace jar {

const char*
SpeakerApplication::name() const
{
    return "Speaker";
}

const char*
SpeakerApplication::contextId()
{
    return "SPKR";
}

const char*
SpeakerApplication::contextDesc()
{
    return "J.A.R.V.I.S Speaker Context";
}

void
SpeakerApplication::initialize(Application& application)
{
    addSubsystem(std::make_unique<SpeakerSubsystem>());

    Application::initialize(application);
}

} // namespace jar

APP_MAIN(jar::SpeakerApplication)
