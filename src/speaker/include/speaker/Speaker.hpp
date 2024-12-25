#pragma once

#include "speaker/IPlayer.hpp"
#include "speaker/IPlayerFactory.hpp"
#include "speaker/ISpeaker.hpp"
#include "speaker/ISpeechSynthesizePool.hpp"
#include "speaker/Types.hpp"

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
    synthesizeText(std::string_view text, std::string_view lang) final;

    void
    synthesizeSsml(std::string_view ssml, std::string_view lang) final;

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
