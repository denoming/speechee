// Copyright 2025 Denys Asauliak
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
