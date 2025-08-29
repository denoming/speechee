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

#include "speechee/MockPlayer.hpp"

using namespace testing;

namespace jar {

MockPlayer::MockPlayer()
{
    ON_CALL(*this, state).WillByDefault(Return(PlayState::Idle));

    ON_CALL(*this, start).WillByDefault(Return(true));

    ON_CALL(*this, play).WillByDefault(Return(true));
}

void
MockPlayer::triggerStateUpdate(PlayState state) const
{
    _onStateUpdateSig(state);
}

IPlayer::OnStateUpdateSignal
MockPlayer::onStateUpdate()
{
    return _onStateUpdateSig;
}

} // namespace jar
