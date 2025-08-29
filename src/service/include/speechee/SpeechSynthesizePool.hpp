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

#include "speechee/ISpeechSynthesizePool.hpp"
#include "speechee/SpeechSynthesizeTask.hpp"

#include <condition_variable>
#include <list>
#include <thread>
#include <vector>

namespace jar {

class ITextToSpeechClient;

class SpeechSynthesizePool final : public ISpeechSynthesizePool {
public:
    explicit SpeechSynthesizePool(ITextToSpeechClient& client, std::size_t threads);

    ~SpeechSynthesizePool() override;

    void
    synthesizeText(std::string_view text,
                   std::string_view lang,
                   std::move_only_function<OnDone> callback) override;

    void
    synthesizeSsml(std::string_view ssml,
                   std::string_view lang,
                   std::move_only_function<OnDone> callback) override;

private:
    using Tasks = std::list<std::unique_ptr<SpeechSynthesizeTask>>;

    void
    setUp(std::size_t threads);

    void
    tearDown();

    void
    threadFn(std::stop_token stoken);

    void
    pushTask(std::unique_ptr<SpeechSynthesizeTask> task);

    bool
    waitTask(std::stop_token stoken);

    std::unique_ptr<SpeechSynthesizeTask>
    nextTask();

private:
    ITextToSpeechClient& _client;
    Tasks _tasks;
    mutable std::mutex _dataGuard;
    std::condition_variable_any _dataPresent;
    std::vector<std::jthread> _threads;
};

} // namespace jar
