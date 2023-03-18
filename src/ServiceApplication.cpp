#include "ServiceApplication.hpp"

#include "speaker/SpeakerSubsystem.hpp"

namespace jar {

const char*
ServiceApplication::name() const
{
    return "Speaker";
}

const char*
ServiceApplication::contextId()
{
    return "SPKR";
}

const char*
ServiceApplication::contextDesc()
{
    return "J.A.R.V.I.S Speaker Context";
}

void
ServiceApplication::initialize(Application& application)
{
    addSubsystem(std::make_unique<SpeakerSubsystem>());

    Application::initialize(application);
}

} // namespace jar

APP_MAIN(jar::ServiceApplication)
