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

#include <gmock/gmock.h>

namespace jar {

class MockPlayer : public IPlayer {
public:
    MockPlayer();

    void
    triggerStateUpdate(PlayState state) const;

    OnStateUpdateSignal
    onStateUpdate() override;

public:
    MOCK_METHOD(PlayState, state, (), (const, override));

    MOCK_METHOD(bool, start, (), (override));

    MOCK_METHOD(void, stop, (), (override));

    MOCK_METHOD(bool, play, (std::string_view audio), (override));

private:
    OnStateUpdateSignal _onStateUpdateSig;
};

} // namespace jar
