#include "speaker/PlayerLoop.hpp"

#include <boost/assert.hpp>

namespace jar {

PlayerLoop::PlayerLoop()
{
    Glib::init();
}

PlayerLoop::~PlayerLoop()
{
    stop();
}

bool
PlayerLoop::active() const
{
    return _thread.joinable();
}

void
PlayerLoop::start()
{
    if (active()) {
        throw std::runtime_error("Loop already started");
    }

    _mainLoop = Glib::MainLoop::create();
    BOOST_ASSERT(_mainLoop);

    _thread = std::jthread{[loop = _mainLoop]() { loop->run(); }};
    BOOST_ASSERT(_thread.joinable());
}

void
PlayerLoop::stop()
{
    if (_mainLoop) {
        if (_mainLoop->is_running()) {
            _mainLoop->quit();
            if (_thread.joinable()) {
                _thread.join();
            }
        }
        _mainLoop.reset();
    }
}

sigc::connection
PlayerLoop::onIdle(const sigc::slot<bool()>& slot) const
{
    BOOST_ASSERT(_mainLoop);
    if (_mainLoop) {
        Glib::SignalIdle idle{unwrap(_mainLoop->get_context())};
        return idle.connect(slot);
    }
    return {};
}

void
PlayerLoop::onIdleOnce(const sigc::slot<void()>& slot) const
{
    BOOST_ASSERT(_mainLoop);
    if (_mainLoop) {
        Glib::SignalIdle idle{unwrap(_mainLoop->get_context())};
        idle.connect_once(slot);
    }
}

} // namespace jar
