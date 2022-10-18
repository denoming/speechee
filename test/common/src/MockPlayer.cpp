#include "test/MockPlayer.hpp"

namespace jar {

void
MockPlayer::triggerStateUpdate(PlayState state)
{
    _onStateUpdateSig(state);
}

sigc::connection
MockPlayer::onStateUpdate(OnStateUpdateSignal::slot_type&& slot)
{
    return _onStateUpdateSig.connect(std::move(slot));
}

} // namespace jar
