#include "speech/Speaker.hpp"

#include "jarvis/Logger.hpp"
#include "speech/Player.hpp"
#include "speech/SpeechSynthesizePool.hpp"

static std::uint64_t
getId()
{
    static std::uint64_t id{0};
    return ++id;
}

namespace jar {

Speaker::Speaker(ISpeechSynthesizePool& synthesizePool, IPlayer& player)
    : _synthesizePool{synthesizePool}
    , _player{player}
{
    _onPlayerStateCon = _player.onStateUpdate(std::bind_front(&Speaker::onPlayerStateUpdate, this));
}

Speaker::~Speaker()
{
    _onPlayerStateCon.disconnect();
}

void
Speaker::synthesizeText(std::string_view text, std::string_view lang)
{
    _synthesizePool.synthesizeText(
        text, lang, [this, id = createRequest()](std::string audio, std::error_code error) {
            onSynthesizeDone(id, std::move(audio), error);
        });
}

void
Speaker::synthesizeSsml(std::string_view ssml, std::string_view lang)
{
    _synthesizePool.synthesizeSsml(
        ssml, lang, [this, id = createRequest()](std::string audio, std::error_code error) {
            onSynthesizeDone(id, std::move(audio), error);
        });
}

uint64_t
Speaker::createRequest()
{
    const auto id = getId();
    std::unique_lock lock{_guard};
    auto& request = _requests.emplace_back();
    request.id = id;
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
Speaker::onSynthesizeDone(std::uint64_t id, std::string audio, std::error_code errorCode)
{
    std::lock_guard lock{_guard};

    auto [ok, requestIt] = findRequest(id);
    if (!ok) {
        LOGE("Request with <{}> id not found", id);
        playNext();
        return;
    }

    if (!errorCode) {
        requestIt->done = true;
        requestIt->audio = std::move(audio);
    } else {
        LOGE("Request <{}> has failed with <{}> error", id, errorCode.message());
        _requests.erase(requestIt);
    }

    playNext();
}

void
Speaker::onPlayerStateUpdate(PlayState state)
{
    if (state == PlayState::Idle) {
        playNext();
    }
}

void
Speaker::playNext()
{
    if (_player.state() == PlayState::Busy) {
        return;
    }

    std::lock_guard lock{_guard};
    if (!_requests.empty()) {
        const auto& next = _requests.front();
        if (next.done) {
            _player.play(next.audio);
            _requests.pop_front();
        }
    }
}

} // namespace jar