#pragma once

#include <sigc++/connection.h>
#include <sigc++/slot.h>

namespace jar {

class IPlayerLoop {
public:
    virtual ~IPlayerLoop() = default;

    [[nodiscard]] virtual bool
    active() const
        = 0;

    virtual void
    start()
        = 0;

    virtual void
    stop()
        = 0;

    [[nodiscard]] virtual sigc::connection
    onIdle(const sigc::slot<bool()>& slot) const
        = 0;
};

} // namespace jar