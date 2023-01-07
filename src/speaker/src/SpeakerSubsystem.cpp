#include "speaker/SpeakerSubsystem.hpp"

#include "jarvis/Logger.hpp"
#include "speaker/Player.hpp"
#include "speaker/Speaker.hpp"
#include "speaker/SpeakerService.hpp"
#include "speaker/SpeechSynthesizePool.hpp"
#include "speaker/TextToSpeechClient.hpp"

#include <boost/assert.hpp>

namespace jar {

class SpeakerSubsystem::Impl {
public:
    void
    initialize(Application& application)
    {
        _client = std::make_unique<TextToSpeechClient>();
        _synthesizePool = std::make_unique<SpeechSynthesizePool>(*_client, 2);
        _player = std::make_unique<Player>();
        _speaker = std::make_unique<Speaker>(*_synthesizePool, *_player);
        _speakerService = std::make_unique<SpeakerService>(*_speaker);
    }

    void
    setUp(Application& application)
    {
        BOOST_ASSERT(_player);
        if (!_player->initialize()) {
            LOGE("Failed to initialize player");
        }

        BOOST_ASSERT(_speakerService);
        _speakerService->start();
    }

    void
    tearDown()
    {
        BOOST_ASSERT(_speakerService);
        _speakerService->stop();

        BOOST_ASSERT(_player);
        _player->finalize();
    }

    void
    finalize()
    {
        _speakerService.reset();
        _speaker.reset();
        _player.reset();
        _synthesizePool.reset();
        _client.reset();
    }

    [[nodiscard]] const ISpeaker&
    speaker() const
    {
        BOOST_ASSERT(_speaker);
        return *_speaker;
    }

    [[nodiscard]] ISpeaker&
    speaker()
    {
        BOOST_ASSERT(_speaker);
        return *_speaker;
    }

private:
    Speaker::Ptr _speaker;
    SpeakerService::Ptr _speakerService;
    SpeechSynthesizePool::Ptr _synthesizePool;
    TextToSpeechClient::Ptr _client;
    Player::Ptr _player;
};

SpeakerSubsystem::SpeakerSubsystem()
    : _impl{std::make_unique<Impl>()}
{
}

SpeakerSubsystem::~SpeakerSubsystem() = default;

const char*
SpeakerSubsystem::name() const
{
    return "Speech";
}

void
SpeakerSubsystem::initialize(Application& application)
{
    Subsystem::initialize(application);

    BOOST_ASSERT(_impl);
    _impl->initialize(application);
}

void
SpeakerSubsystem::setUp(Application& application)
{
    Subsystem::setUp(application);

    BOOST_ASSERT(_impl);
    _impl->setUp(application);
}

void
SpeakerSubsystem::tearDown()
{
    Subsystem::tearDown();

    BOOST_ASSERT(_impl);
    _impl->tearDown();
}

void
SpeakerSubsystem::finalize()
{
    Subsystem::finalize();

    BOOST_ASSERT(_impl);
    _impl->finalize();
}

} // namespace jar
