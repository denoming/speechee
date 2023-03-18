#pragma once

#include <sdbus-c++/IProxy.h>

namespace jar {

class SpeakerProxy {
public:
    explicit SpeakerProxy(sdbus::IProxy& proxy);

    virtual ~SpeakerProxy() = default;
};

} // namespace jar