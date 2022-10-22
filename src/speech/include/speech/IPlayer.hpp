#pragma once

#include "speech/Types.hpp"

#include <sigc++/connection.h>
#include <sigc++/signal.h>

#include <memory>
#include <string>

namespace jar {

class IPlayer {
public:
    using Ptr = std::unique_ptr<IPlayer>;

    /* Signatures */
    using OnStateUpdate = void(PlayState state);
    /* Signals */
    using OnStateUpdateSignal = sigc::signal<OnStateUpdate>;

    virtual ~IPlayer() = default;

    [[nodiscard]] virtual PlayState
    state() const
        = 0;

    [[nodiscard]] virtual bool
    initialize()
        = 0;

    virtual void
    finalize()
        = 0;

    virtual bool
    play(std::string_view audio)
        = 0;

    [[nodiscard]] virtual sigc::connection
    onStateUpdate(OnStateUpdateSignal::slot_type&& slot)
        = 0;
};

} // namespace jar