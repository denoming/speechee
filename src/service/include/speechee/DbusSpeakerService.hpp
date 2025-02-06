#pragma once

#include "speechee/ISpeakerService.hpp"

#include <jarvisto/network/AvailabilitySubject.hpp>

#include <memory>

namespace jar {

class ISpeaker;

class DbusSpeakerService final : public ISpeakerService, public AvailabilitySubject {
public:
    explicit DbusSpeakerService(ISpeaker& speaker);

    ~DbusSpeakerService() override;

    [[nodiscard]] bool
    start() override;

    void
    stop() override;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar
