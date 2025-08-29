// Copyright 2025 Denys Asauliak
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
