#include "speech/SpeechSubsystem.hpp"

#include "jarvis/Logger.hpp"
#include "speech/Player.hpp"
#include "speech/Speaker.hpp"
#include "speech/SpeechSynthesizePool.hpp"
#include "speech/TextToSpeechClient.hpp"

namespace jar {

class SpeechSubsystem::Impl {
public:
    void
    initialize(Application& application)
    {
        _client = std::make_unique<TextToSpeechClient>();
        _synthesizePool = std::make_unique<SpeechSynthesizePool>(*_client, 2);
        _player = std::make_unique<Player>();
        _speaker = std::make_unique<Speaker>(*_synthesizePool, *_player);
    }

    void
    setUp(Application& application)
    {
        if (!_player->initialize()) {
            LOGE("Failed to initialize player");
        }
    }

    void
    tearDown()
    {
        _player->finalize();
    }

    void
    finalize()
    {
        _speaker.reset();
        _player.reset();
        _synthesizePool.reset();
        _client.reset();
    }

private:
    ISpeaker::Ptr _speaker;
    ISpeechSynthesizePool::Ptr _synthesizePool;
    ITextToSpeechClient::Ptr _client;
    IPlayer::Ptr _player;
};

SpeechSubsystem::SpeechSubsystem()
    : _impl{std::make_unique<Impl>()}
{
}

SpeechSubsystem::~SpeechSubsystem() = default;

const char*
SpeechSubsystem::name() const
{
    return "Speech";
}

void
SpeechSubsystem::initialize(Application& application)
{
    Subsystem::initialize(application);

    _impl->initialize(application);
}

void
SpeechSubsystem::setUp(Application& application)
{
    Subsystem::setUp(application);

    _impl->setUp(application);
}

void
SpeechSubsystem::tearDown()
{
    _impl->tearDown();

    Subsystem::tearDown();
}

void
SpeechSubsystem::finalize()
{
    _impl->finalize();

    Subsystem::finalize();
}

} // namespace jar
