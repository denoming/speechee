#pragma once

#include "jarvis-speaker-client-glue.h"

namespace jar {

class SpeakerClientImpl : public sdbus::ProxyInterfaces<org::denoming::jarvis::speaker_proxy> {
public:
    SpeakerClientImpl(std::string destination, std::string objectPath);

    ~SpeakerClientImpl();
};

} // namespace jar