#pragma once

#include <thread>

#include <glibmm.h>

namespace jar {

class PlayerLoop {
public:
    PlayerLoop();

    ~PlayerLoop();

    [[nodiscard]] bool
    active() const;

    void
    start();

    void
    stop();

    [[nodiscard]] sigc::connection
    onIdle(const sigc::slot<bool()>& slot) const;

    void
    onIdleOnce(const sigc::slot<void()>& slot) const;

private:
    std::jthread _thread;
    Glib::RefPtr<Glib::MainLoop> _mainLoop;
};

} // namespace jar
