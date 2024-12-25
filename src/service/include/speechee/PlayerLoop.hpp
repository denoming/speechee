#pragma once

#include "speechee/IPlayerLoop.hpp"

#include <glibmm/main.h>

#include <thread>

namespace jar {

class PlayerLoop final : public IPlayerLoop {
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
    onIdle(const sigc::slot<bool()>& slot) override;

private:
    void
    quit() const;

private:
    std::jthread _thread;
    Glib::RefPtr<Glib::MainLoop> _mainLoop;
};

} // namespace jar
