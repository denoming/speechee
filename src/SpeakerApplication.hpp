#pragma once

#include "jarvis/Application.hpp"

namespace jar {

class SpeakerApplication final : public jar::Application {
public:
    SpeakerApplication() = default;

    [[nodiscard]] const char*
    name() const final;

private:
    void
    initialize(Application& application) final;

    void
    proceed() final;
};

} // namespace jar