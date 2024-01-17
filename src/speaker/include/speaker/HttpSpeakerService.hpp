#pragma once

#include <jarvisto/AvailabilitySubject.hpp>

#include <cstdint>
#include <memory>

namespace jar {

class ISpeaker;

class HttpSpeakerService final : public AvailabilitySubject {
public:
    explicit HttpSpeakerService(std::size_t concurrency, std::uint16_t port, ISpeaker& speaker);

    ~HttpSpeakerService() final;

    bool
    start();

    void
    stop();

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar