#pragma once

#include <memory>

namespace jar {

class ISpeaker;

class DbusSpeakerService final {
public:
    explicit DbusSpeakerService(ISpeaker& speaker);

    ~DbusSpeakerService();

    void
    start();

    void
    stop();

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar
