#pragma once

#include "jarvis/Application.hpp"

namespace jar {

class ServiceApplication final : public jar::Application {
public:
    ServiceApplication() = default;

    [[nodiscard]] const char*
    name() const final;

private:
    void
    initialize(Application& application) final;

    void
    proceed() final;
};

} // namespace jar