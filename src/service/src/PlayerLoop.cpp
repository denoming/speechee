#include "speechee/PlayerLoop.hpp"

#include <boost/assert.hpp>
#include <glibmm/init.h>
#include <glibmm/wrap.h>

namespace jar {

PlayerLoop::PlayerLoop()
{
    Glib::init();
}

PlayerLoop::~PlayerLoop()
{
    quit();
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
    quit();
}

sigc::connection
PlayerLoop::onIdle(const sigc::slot<bool()>& slot)
{
    BOOST_ASSERT(_mainLoop);
    if (_mainLoop) {
        const auto src = Glib::IdleSource::create();
        auto c = src->connect(slot);
        src->attach(_mainLoop->get_context());
        return c;
    }
    return {};
}

void
PlayerLoop::quit() const
{
    if (_mainLoop) {
        if (_mainLoop->is_running()) {
            _mainLoop->quit();
        }
    }
}

} // namespace jar
