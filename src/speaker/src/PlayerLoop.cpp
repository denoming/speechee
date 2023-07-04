#include "speaker/PlayerLoop.hpp"

#include <jarvisto/Logger.hpp>

#include <boost/assert.hpp>

namespace jar {

PlayerLoop::PlayerLoop()
{
    Glib::init();

    _loop = Glib::MainLoop::create();
    BOOST_ASSERT(_loop);

    _thread = std::jthread{[loop = _loop]() {
        LOGD("Player loop: begin");
        loop->run();
        LOGD("Player loop: end");
    }};
}

PlayerLoop::~PlayerLoop()
{
    stop();
}

void
PlayerLoop::stop()
{
    if (!_loop->is_running()) {
        return;
    }

    _loop->quit();
    if (_thread.joinable()) {
        _thread.join();
    }
}

void
PlayerLoop::invoke(const sigc::slot<bool()>& slot)
{
    BOOST_ASSERT(_loop->is_running());
    _loop->get_context()->invoke(slot);
}

} // namespace jar
