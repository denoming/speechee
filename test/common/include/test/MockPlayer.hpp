#pragma once

#include "speaker/IPlayer.hpp"

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
