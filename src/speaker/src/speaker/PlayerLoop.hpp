#pragma once

#include <thread>

#include <glibmm.h>

namespace jar {

class PlayerLoop {
public:
    PlayerLoop();

    ~PlayerLoop();

    void
    stop();

    void
    invoke(const sigc::slot<bool()>& slot);

private:
    std::jthread _thread;
    Glib::RefPtr<Glib::MainLoop> _loop;
};

} // namespace jar
