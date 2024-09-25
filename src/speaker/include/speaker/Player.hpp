#pragma once

#include "speaker/IPlayer.hpp"

#include <memory>

namespace jar {

class Player final : public IPlayer {
public:
    Player();

    ~Player() override;

    [[nodiscard]] PlayState
    state() const override;

    [[nodiscard]] bool
    start() override;

    void
    stop() override;

    bool
    play(std::string_view audio) override;

    [[nodiscard]] sigc::connection
    onStateUpdate(OnStateUpdateSignal::slot_type&&) override;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar
