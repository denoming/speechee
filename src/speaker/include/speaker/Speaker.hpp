#pragma once

#include "speaker/ISpeaker.hpp"
#include "speaker/Types.hpp"

#include <sigc++/connection.h>

#include <list>
#include <mutex>
#include <string>

#include <system_error>

namespace jar {

class ISpeechSynthesizePool;
class IPlayer;

class Speaker final : public ISpeaker {
public:
    Speaker(ISpeechSynthesizePool& synthesizePool, IPlayer& player);

    ~Speaker() final;

    void
    synthesizeText(std::string_view text, std::string_view lang) final;

    void
    synthesizeSsml(std::string_view ssml, std::string_view lang) final;

private:
    struct Request {
        std::uint64_t id{0};
        bool done{false};
        std::string audio;
    };
    using Requests = std::list<Request>;

private:
    using MatchResult = std::tuple<bool, Requests::iterator>;

    [[nodiscard]] uint64_t
    createRequest();

    [[nodiscard]] MatchResult
    findRequest(std::uint64_t id);

    void
    onSynthesizeDone(std::uint64_t id, std::string audio, std::error_code errorCode);

    void
    onPlayerStateUpdate(PlayState state);

    void
    playNext();

private:
    mutable std::recursive_mutex _guard;
    ISpeechSynthesizePool& _synthesizePool;
    IPlayer& _player;
    sigc::connection _onPlayerStateCon;
    Requests _requests;
};

} // namespace jar
