#pragma once

#include <sdbus-c++/IProxy.h>

namespace jar {

class DbusSpeakerProxy {
public:
    explicit DbusSpeakerProxy(sdbus::IProxy& proxy);

    virtual ~DbusSpeakerProxy() = default;

    void registerProxy();
};

} // namespace jar