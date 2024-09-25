#pragma once

#include "speaker/ISpeaker.hpp"
#include "speaker/Types.hpp"

#include <list>
#include <mutex>
#include <string>
#include <string_view>

namespace jar {

class ISpeechSynthesizePool;
class IPlayer;

class Speaker final : public ISpeaker {
public:
    Speaker(ISpeechSynthesizePool& synthesizePool, IPlayer& player);

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
    onSynthesizeDone(std::uint64_t id, std::string audio, std::exception_ptr exception);

    void
    playAudio();

private:
    mutable std::recursive_mutex _guard;
    ISpeechSynthesizePool& _synthesizePool;
    IPlayer& _player;
    Requests _requests;
};

} // namespace jar
