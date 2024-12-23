#pragma once

#include <jarvisto/core/Application.hpp>

namespace jar {

class ServiceApplication final : public jar::Application {
public:
    ServiceApplication() = default;

    [[nodiscard]] const char*
    name() const final;

private:
    void
    proceed() final;

    void
    initialize(Application& application) final;
};

} // namespace jar
