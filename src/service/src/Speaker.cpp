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

#include "speechee/Speaker.hpp"

#include <jarvisto/core/Logger.hpp>

#include "speechee/Formatters.hpp"
#include "speechee/Player.hpp"
#include "speechee/SpeechSynthesizePool.hpp"

#include <sigc++/adaptors/track_obj.h>

static std::uint64_t
getId()
{
    static std::uint64_t id{0};
    return ++id;
}

namespace jar {

Speaker::Speaker(ISpeechSynthesizePool& synthesizePool, IPlayerFactory& playerFactory)
    : _synthesizePool{synthesizePool}
    , _playerFactory{playerFactory}
{
}

Speaker::~Speaker() = default;

void
Speaker::synthesizeText(std::string_view text, std::string_view lang)
{
    _synthesizePool.synthesizeText(
        text, lang, [this, id = createRequest()](std::string audio, std::exception_ptr exception) {
            onSynthesizeDone(id, std::move(audio), std::move(exception));
        });
}

void
Speaker::synthesizeSsml(std::string_view ssml, std::string_view lang)
{
    _synthesizePool.synthesizeSsml(
        ssml, lang, [this, id = createRequest()](std::string audio, std::exception_ptr exception) {
            onSynthesizeDone(id, std::move(audio), std::move(exception));
        });
}

uint64_t
Speaker::createRequest()
{
    const auto id = getId();
    std::unique_lock lock{_guard};
    auto& r = _requests.emplace_back();
    r.id = id;
    r.player = _playerFactory.create();
    std::ignore = r.player->onStateUpdate().connect(
        track_obj([this, id](const auto state) { onPlayUpdate(id, state); }, r));
    lock.unlock();
    return id;
}

Speaker::MatchResult
Speaker::findRequest(std::uint64_t id)
{
    auto it = std::find_if(
        _requests.begin(), _requests.end(), [id](const auto& r) { return (r.id == id); });
    return std::make_tuple(it != _requests.end(), it);
}

void
Speaker::onSynthesizeDone(std::uint64_t id, std::string audio, std::exception_ptr exception)
{
    std::lock_guard lock{_guard};

    auto [ok, requestIt] = findRequest(id);
    if (not ok) {
        LOGE("Request with <{}> id not found", id);
        return;
    }

    if (exception) {
        _requests.erase(requestIt);
        try {
            rethrow_exception(std::move(exception));
        } catch (const std::exception& e) {
            LOGE("Unable to synthesize <{}> request: {}", id, e.what());
        }
    } else {
        const Request& r = *requestIt;
        if (not r.player->start()) {
            LOGE("Unable to start player of <{}> request", id);
            _requests.erase(requestIt);
            return;
        }
        if (not r.player->play(audio)) {
            LOGE("Unable to play audio of <{}> request", id);
            r.player->stop();
            _requests.erase(requestIt);
            return;
        }
        LOGD("Playing <{}> request", id);
    }
}

void
Speaker::onPlayUpdate(std::uint64_t id, PlayState state)
{
    std::lock_guard lock{_guard};

    auto [ok, requestIt] = findRequest(id);
    if (not ok) {
        LOGE("Request with <{}> id not found", id);
        return;
    }

    if (Request& r = *requestIt; r.state == PlayState::Busy and state == PlayState::Idle) {
        LOGD("Playing of <{}> request was done", id);
        r.player->stop();
        _requests.erase(requestIt);
    } else {
        r.state = state;
    }
}

} // namespace jar
