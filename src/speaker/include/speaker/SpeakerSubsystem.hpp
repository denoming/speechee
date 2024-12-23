#pragma once

#include <jarvisto/core/Subsystem.hpp>

#include <memory>

namespace jar {

class ISpeaker;

class SpeakerSubsystem final : public Subsystem {
public:
    SpeakerSubsystem();

    ~SpeakerSubsystem() final;

    [[nodiscard]] const char*
    name() const final;

    void
    initialize(Application& application) final;

    void
    setUp(Application& application) final;

    void
    tearDown() final;

    void
    finalize() final;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar
