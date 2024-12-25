#pragma once

#include "speechee/Types.hpp"

#include <fmt/format.h>
#include <fmt/std.h>

template<>
struct fmt::formatter<jar::PlayState> : fmt::formatter<std::string_view> {
    template<typename FormatContext>
    auto
    format(const jar::PlayState state, FormatContext& context) const
    {
        std::string_view output{"Unknown"};
        switch (state) {
        case jar::PlayState::Null:
            output = "Null";
            break;
        case jar::PlayState::Idle:
            output = "Idle";
            break;
        case jar::PlayState::Busy:
            output = "Busy";
            break;
        case jar::PlayState::Error:
            output = "Error";
            break;
        }
        return formatter<string_view>::format(output, context);
    }
};
