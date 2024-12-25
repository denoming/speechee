#pragma once

namespace jar {

class GstInitializer {
public:
    GstInitializer() = default;

    [[nodiscard]] bool
    initialize();
};

} // namespace jar