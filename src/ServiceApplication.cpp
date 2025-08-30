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

#include "ServiceApplication.hpp"

#include "speechee/ServiceLogger.hpp"
#include "speechee/SpeakerSubsystem.hpp"

#include <jarvisto/core/SignalHandler.hpp>

namespace jar {

const char*
ServiceApplication::name() const
{
    return "Speechee";
}

void
ServiceApplication::proceed()
{
    const SignalHandler handler{SIGINT, SIGTERM};
    std::ignore = handler.wait();
}

void
ServiceApplication::initialize(Application& application)
{
    ServiceLogger logger{"SPEECHEE"};
    logger.create("MAIN", SPDLOG_LEVEL_DEBUG);

    addSubsystem(std::make_unique<SpeakerSubsystem>());

    Application::initialize(application);
}

} // namespace jar

APP_MAIN(jar::ServiceApplication)
