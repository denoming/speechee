#pragma once

#include "speaker/AvailabilitySubject.hpp"

#include <memory>

namespace jar {

class ISpeaker;

class DbusSpeakerService final : public AvailabilitySubject {
public:
    explicit DbusSpeakerService(ISpeaker& speaker);

    ~DbusSpeakerService() final;

    [[nodiscard]] bool
    start();

    void
    stop();

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar
