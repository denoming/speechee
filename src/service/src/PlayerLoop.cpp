#include "speechee/PlayerLoop.hpp"

#include <gsl/gsl-lite.hpp>

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
    gsl_Assert(_mainLoop);

    _thread = std::jthread{[loop = _mainLoop]() { loop->run(); }};
    gsl_Assert(_thread.joinable());
}

void
PlayerLoop::stop()
{
    quit();
}

sigc::connection
PlayerLoop::onIdle(const sigc::slot<bool()>& slot)
{
    gsl_Assert(_mainLoop);
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
