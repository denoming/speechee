#pragma once

#include "speaker/IPlayer.hpp"

#include <gmock/gmock.h>

namespace jar {

class MockPlayer : public IPlayer {
public:
    void
    triggerStateUpdate(PlayState state);

    sigc::connection
    onStateUpdate(OnStateUpdateSignal::slot_type&& slot) override;

public:
    MOCK_METHOD(PlayState, state, (), (const, override));

    MOCK_METHOD(bool, start, (), (override));

    MOCK_METHOD(void, stop, (), (override));

    MOCK_METHOD(bool, play, (std::string_view audio), (override));

private:
    OnStateUpdateSignal _onStateUpdateSig;
};

} // namespace jar
