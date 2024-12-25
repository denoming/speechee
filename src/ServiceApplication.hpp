#pragma once

#include <jarvisto/core/Application.hpp>

namespace jar {

class ServiceApplication final : public Application {
public:
    ServiceApplication() = default;

    [[nodiscard]] const char*
    name() const override;

private:
    void
    proceed() override;

    void
    initialize(Application& application) override;
};

} // namespace jar
