#pragma once

namespace jar {

class ISpeakerService {
public:
    virtual ~ISpeakerService() = default;

    [[nodiscard]] virtual bool
    start()
        = 0;

    virtual void
    stop()
        = 0;
};

} // namespace jar