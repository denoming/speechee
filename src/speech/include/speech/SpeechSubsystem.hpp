#pragma once

#include "jarvis/Subsystem.hpp"

#include <memory>

namespace jar {

class ISpeaker;

class SpeechSubsystem final : public Subsystem {
public:
    SpeechSubsystem();

    ~SpeechSubsystem() final;

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