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
