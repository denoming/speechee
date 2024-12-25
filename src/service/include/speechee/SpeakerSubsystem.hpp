#pragma once

#include <jarvisto/core/Subsystem.hpp>

#include <memory>

namespace jar {

class ISpeaker;

class SpeakerSubsystem final : public Subsystem {
public:
    SpeakerSubsystem();

    ~SpeakerSubsystem() override;

    [[nodiscard]] const char*
    name() const override;

    void
    initialize(Application& application) override;

    void
    setUp(Application& application) override;

    void
    tearDown() override;

    void
    finalize() override;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar
