#pragma once

#include <string>

#include <sigc++/connection.h>
#include <sigc++/signal.h>

namespace jar {

enum class PlayState {
    Null, Idle, Busy, Error
};

class IPlayer {
public:
    /* Signatures */
    using OnStateUpdate = void(PlayState state);
    /* Signals */
    using OnStateUpdateSignal = sigc::signal<OnStateUpdate>;

    virtual ~IPlayer() = default;

    [[nodiscard]] virtual PlayState
    state() const = 0;

    virtual bool
    play(std::string_view audio)
        = 0;

    [[nodiscard]] virtual sigc::connection
    onStateUpdate(OnStateUpdateSignal::slot_type&&)
        = 0;
};

} // namespace jar