#pragma once

#include "jarvis/Application.hpp"

namespace jar {

class ServiceApplication final : public jar::Application {
public:
    ServiceApplication() = default;

    [[nodiscard]] const char*
    name() const final;

private:
    const char*
    contextId() final;

    const char*
    contextDesc() final;

    void
    initialize(Application& application) final;
};

} // namespace jar
