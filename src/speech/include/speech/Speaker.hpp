#pragma once

#include <list>
#include <string>
#include <string_view>
#include <system_error>

#include "speech/IPlayer.hpp"

namespace jar {

class ISpeechSynthesizePool;

class Speaker {
public:
    Speaker(ISpeechSynthesizePool& synthesizePool, IPlayer& player);

    ~Speaker();

    void
    synthesizeText(std::string_view text, std::string_view lang);

    void
    synthesizeSsml(std::string_view ssml, std::string_view lang);

private:
    struct Request {
        std::uint64_t id{0};
        bool done{false};
        std::string audio;
    };
    using Requests = std::list<Request>;

private:
    void
    onSynthesizeDone(std::uint64_t id, std::string audio, std::error_code errorCode);

    void
    onPlayerStateUpdate(PlayState state);

    void
    playNext();

private:
    ISpeechSynthesizePool& _synthesizePool;
    IPlayer& _player;
    sigc::connection _onPlayerStateCon;
    Requests _requests;
};

} // namespace jar