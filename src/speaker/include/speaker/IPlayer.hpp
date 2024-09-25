#pragma once

#include "speaker/Types.hpp"

#include <sigc++/signal.h>

#include <string>

namespace jar {

class IPlayer {
public:
    /* Signatures */
    using OnStateUpdate = void(PlayState state);
    /* Signals */
    using OnStateUpdateSignal = sigc::signal<OnStateUpdate>;

    virtual ~IPlayer() = default;

    [[nodiscard]] virtual PlayState
    state() const
        = 0;

    [[nodiscard]] virtual bool
    start()
        = 0;

    virtual void
    stop()
        = 0;

    virtual bool
    play(std::string_view audio)
        = 0;

    [[nodiscard]] virtual sigc::connection
    onStateUpdate(OnStateUpdateSignal::slot_type&& slot)
        = 0;
};

} // namespace jar
