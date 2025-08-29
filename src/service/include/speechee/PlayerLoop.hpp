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
