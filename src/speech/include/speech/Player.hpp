#pragma once

#include "speech/IPlayer.hpp"

#include <memory>
#include <string>

namespace jar {

class Player final : public IPlayer {
public:
    Player();

    ~Player() final;

    [[nodiscard]] bool
    initialize();

    [[nodiscard]] bool
    initialize(int* argc, char** argv[]);

    void
    finalize();

    [[nodiscard]] PlayState
    state() const final;

    bool
    play(std::string_view audio) final;

    [[nodiscard]] sigc::connection
    onStateUpdate(OnStateUpdateSignal::slot_type&&) final;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar