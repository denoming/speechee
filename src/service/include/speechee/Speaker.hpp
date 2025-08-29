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

#include "speechee/IPlayer.hpp"
#include "speechee/IPlayerFactory.hpp"
#include "speechee/ISpeaker.hpp"
#include "speechee/ISpeechSynthesizePool.hpp"
#include "speechee/Types.hpp"

#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>

#include <sigc++/trackable.h>

namespace jar {

class ISpeechSynthesizePool;

class Speaker final : public ISpeaker {
public:
    explicit Speaker(ISpeechSynthesizePool& synthesizePool, IPlayerFactory& playerFactory);

    ~Speaker() override;

    void
    synthesizeText(std::string_view text, std::string_view lang) override;

    void
    synthesizeSsml(std::string_view ssml, std::string_view lang) override;

private:
    struct Request : sigc::trackable {
        std::uint64_t id{0};
        PlayState state{};
        std::shared_ptr<IPlayer> player;
    };
    using Requests = std::list<Request>;

private:
    using MatchResult = std::tuple<bool, Requests::iterator>;

    [[nodiscard]] uint64_t
    createRequest();

    [[nodiscard]] MatchResult
    findRequest(std::uint64_t id);

    void
    onSynthesizeDone(std::uint64_t id, std::string audio, std::exception_ptr exception);

    void
    onPlayUpdate(std::uint64_t id, PlayState state);

private:
    mutable std::recursive_mutex _guard;
    ISpeechSynthesizePool& _synthesizePool;
    IPlayerFactory& _playerFactory;
    Requests _requests;
};

} // namespace jar
