#pragma once

#include "speaker/IPlayer.hpp"
#include "speaker/IPlayerLoop.hpp"

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
