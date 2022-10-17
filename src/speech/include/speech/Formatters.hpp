#pragma once

#include "speech/Types.hpp"

#include <fmt/format.h>

template<>
struct fmt::formatter<jar::PlayState> : fmt::formatter<std::string_view> {
    template<typename FormatContext>
    auto
    format(jar::PlayState state, FormatContext& context) const
    {
        std::string_view output{"Unknown"};
        switch (state) {
        case jar::PlayState::Null:
            output = "PlayState::Null";
            break;
        case jar::PlayState::Idle:
            output = "PlayState::Idle";
            break;
        case jar::PlayState::Busy:
            output = "PlayState::Busy";
            break;
        case jar::PlayState::Error:
            output = "PlayState::Error";
            break;
        }
        return formatter<string_view>::format(output, context);
    }
};

namespace jar {

} // namespace jar