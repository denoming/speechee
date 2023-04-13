#pragma once

#include <memory>

namespace jar {

class ISpeaker;

class SpeakerService final {
public:
    explicit SpeakerService(ISpeaker& speaker);

    ~SpeakerService();

    void
    start();

    void
    stop();

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar
