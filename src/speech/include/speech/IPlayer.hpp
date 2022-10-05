#pragma once

#include <string>

#include <sigc++/connection.h>
#include <sigc++/signal.h>

namespace jar {

class IPlayer {
public:
    enum State { Null, Idle, Busy, Error };

    /* Signatures */
    using OnStateUpdate = void(State state);
    /* Signals */
    using OnStateUpdateSignal = sigc::signal<OnStateUpdate>;

    virtual ~IPlayer() = default;

    [[nodiscard]] virtual State
    state() const = 0;

    virtual bool
    play(std::string_view audio)
        = 0;

    [[nodiscard]] virtual sigc::connection
    onStateUpdate(OnStateUpdateSignal::slot_type&&)
        = 0;
};

} // namespace jar