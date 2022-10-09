#include "speech/Speaker.hpp"

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
    _onPlayerStateCon
        = _player.onStateUpdate([this](PlayState state) { onPlayerStateUpdate(state); });
}

Speaker::~Speaker()
{
    _onPlayerStateCon.disconnect();
}

void
Speaker::synthesizeText(std::string_view text, std::string_view lang)
{
    auto& request = _requests.emplace_back();
    request.id = getId();

    _synthesizePool.synthesizeText(
        text, lang, [this, id = request.id](std::string audio, std::error_code error) {
            onSynthesizeDone(id, std::move(audio), error);
        });
}

void
Speaker::synthesizeSsml(std::string_view ssml, std::string_view lang)
{
    auto& request = _requests.emplace_back();
    request.id = getId();

    _synthesizePool.synthesizeText(
        ssml, lang, [this, id = request.id](std::string audio, std::error_code error) {
            onSynthesizeDone(id, std::move(audio), error);
        });
}

void
Speaker::onSynthesizeDone(std::uint64_t id, std::string audio, std::error_code errorCode)
{
    auto requestIt = std::find_if(
        _requests.begin(), _requests.end(), [id](const Request& r) { return (r.id == id); });
    if (requestIt == _requests.end()) {
        return;
    }

    if (errorCode) {
        _requests.erase(requestIt);
    } else {
        requestIt->done = true;
        requestIt->audio = std::move(audio);
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
    auto& next = _requests.front();
    if (!next.done) {
        return;
    }

    if (_player.state() == PlayState::Busy) {
        return;
    }

    _player.play(next.audio);

    _requests.pop_front();
}

} // namespace jar