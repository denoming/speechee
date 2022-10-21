#pragma once

#include <memory>

namespace jar {

class ISpeaker;

class SpeakerService final {
public:
    using Ptr = std::unique_ptr<SpeakerService>;

    explicit SpeakerService(ISpeaker& speaker);

    ~SpeakerService();

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar