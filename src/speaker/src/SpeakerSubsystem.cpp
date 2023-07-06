#include "speaker/SpeakerSubsystem.hpp"

#include "speaker/DbusSpeakerService.hpp"
#include "speaker/HttpSpeakerService.hpp"
#include "speaker/Player.hpp"
#include "speaker/Speaker.hpp"
#include "speaker/SpeechSynthesizePool.hpp"
#include "tts/TextToSpeechClient.hpp"

#include <jarvisto/Logger.hpp>

#include <boost/assert.hpp>

namespace jar {

class SpeakerSubsystem::Impl {
public:
    inline static unsigned kHttpServiceConcurrency{2U};
    inline static std::uint16_t kHttpServicePort{8080U};

    void
    initialize(Application& application)
    {
        _client = std::make_unique<TextToSpeechClient>();
        _synthesizePool = std::make_unique<SpeechSynthesizePool>(*_client, 2);
        _player = std::make_unique<Player>();
        _speaker = std::make_unique<Speaker>(*_synthesizePool, *_player);
        _dbusService = std::make_unique<DbusSpeakerService>(*_speaker);
        _httpService = std::make_unique<HttpSpeakerService>(
            kHttpServiceConcurrency, kHttpServicePort, *_speaker);
    }

    void
    setUp(Application& application)
    {
        BOOST_ASSERT(_player);
        if (!_player->initialize()) {
            LOGE("Failed to initialize player");
        }

        BOOST_ASSERT(_dbusService);
        _dbusService->start();
        BOOST_ASSERT(_httpService);
        _httpService->start();
    }

    void
    tearDown()
    {
        BOOST_ASSERT(_httpService);
        _httpService->stop();
        BOOST_ASSERT(_dbusService);
        _dbusService->stop();

        BOOST_ASSERT(_player);
        _player->finalize();
    }

    void
    finalize()
    {
        _httpService.reset();
        _dbusService.reset();
        _speaker.reset();
        _player.reset();
        _synthesizePool.reset();
        _client.reset();
    }

private:
    std::unique_ptr<Speaker> _speaker;
    std::unique_ptr<DbusSpeakerService> _dbusService;
    std::unique_ptr<HttpSpeakerService> _httpService;
    std::unique_ptr<SpeechSynthesizePool> _synthesizePool;
    std::unique_ptr<TextToSpeechClient> _client;
    std::unique_ptr<Player> _player;
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
