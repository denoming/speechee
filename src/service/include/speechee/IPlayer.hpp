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

#include <sigc++/signal.h>

#include <string_view>

namespace jar {

class IPlayer {
public:
    /* Signatures */
    using OnStateUpdate = void(PlayState state);
    /* Signals */
    using OnStateUpdateSignal = sigc::signal<OnStateUpdate>;

    virtual ~IPlayer() = default;

    [[nodiscard]] virtual PlayState
    state() const
        = 0;

    [[nodiscard]] virtual bool
    start()
        = 0;

    virtual void
    stop()
        = 0;

    virtual bool
    play(std::string_view audio)
        = 0;

    [[nodiscard]] virtual OnStateUpdateSignal
    onStateUpdate() = 0;
};

} // namespace jar
