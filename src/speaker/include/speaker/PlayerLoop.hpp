#pragma once

#include "speaker/IPlayerLoop.hpp"

#include <glibmm/main.h>

#include <thread>

namespace jar {

class PlayerLoop : public IPlayerLoop {
public:
    PlayerLoop();

    ~PlayerLoop() override;

    [[nodiscard]] bool
    active() const override;

    void
    start() override;

    void
    stop() override;

    [[nodiscard]] sigc::connection
    onIdle(const sigc::slot<bool()>& slot) const override;

private:
    void
    quit() const;

private:
    std::jthread _thread;
    Glib::RefPtr<Glib::MainLoop> _mainLoop;
};

} // namespace jar
