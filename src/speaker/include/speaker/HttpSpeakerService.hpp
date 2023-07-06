#pragma once

#include <memory>

namespace jar {

class ISpeaker;

class HttpSpeakerService {
public:
    explicit HttpSpeakerService(std::size_t concurrency, std::uint16_t port, ISpeaker& speaker);

    ~HttpSpeakerService();

    void
    start();

    void
    stop();

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar