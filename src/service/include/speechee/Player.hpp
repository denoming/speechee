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

#include "speechee/IPlayer.hpp"
#include "speechee/IPlayerLoop.hpp"

#include <memory>

namespace jar {

class Player final : public IPlayer {
public:
    explicit Player(IPlayerLoop& playerLoop);

    ~Player() override;

    [[nodiscard]] PlayState
    state() const override;

    [[nodiscard]] bool
    start() override;

    void
    stop() override;

    bool
    play(std::string_view audio) override;

    [[nodiscard]] OnStateUpdateSignal
    onStateUpdate() override;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar
