#pragma once

#include <jarvisto/network/AvailabilitySubject.hpp>

#include <memory>

namespace jar {

class ISpeaker;

class HttpSpeakerService final : public AvailabilitySubject {
public:
    explicit HttpSpeakerService(std::size_t concurrency, std::uint16_t port, ISpeaker& speaker);

    ~HttpSpeakerService() override;

    bool
    start();

    void
    stop();

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar