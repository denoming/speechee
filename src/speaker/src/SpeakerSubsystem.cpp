#include "speaker/SpeakerSubsystem.hpp"

#include "speaker/AvailabilityObserver.hpp"
#include "speaker/AvailabilityPublisher.hpp"
#include "speaker/DbusSpeakerService.hpp"
#include "speaker/HttpSpeakerService.hpp"
#include "speaker/MqttPublisher.hpp"
#include "speaker/Player.hpp"
#include "speaker/Speaker.hpp"
#include "speaker/SpeechSynthesizePool.hpp"
#include "tts/TextToSpeechClient.hpp"

#include <jarvisto/Logger.hpp>

#include <boost/assert.hpp>

namespace jar {

class SpeakerSubsystem::Impl {
public:
    inline static std::size_t kHttpConcurrency{2U};
    inline static std::uint16_t kHttpPort{8080U};
    inline static std::string kMqttUser{"denys"};
    inline static std::string kMqttPassword{"123456"};
    inline static std::string kMqttServer{"192.168.1.43"};


    void
    initialize(Application& /*application*/)
    {
        _client = std::make_unique<TextToSpeechClient>();
        _synthesizePool = std::make_unique<SpeechSynthesizePool>(*_client, 2);
        _player = std::make_unique<Player>();
        _speaker = std::make_unique<Speaker>(*_synthesizePool, *_player);
        _dbusService = std::make_unique<DbusSpeakerService>(*_speaker);
        _httpService = std::make_unique<HttpSpeakerService>(kHttpConcurrency, kHttpPort, *_speaker);
        _observer = std::make_unique<AvailabilityObserver>("speechee");
        _mqtt = std::make_unique<MqttPublisher>();
        _publisher = std::make_unique<AvailabilityPublisher>(*_mqtt, *_observer);
    }

    void
    setUp(Application& /*application*/)
    {
        BOOST_ASSERT(_mqtt);
        _mqtt->credentials(kMqttUser, kMqttPassword);
        _mqtt->connect(kMqttServer);

        BOOST_ASSERT(_observer);
        _observer->add(*_dbusService);
        _observer->add(*_httpService);

        BOOST_ASSERT(_player);
        if (!_player->initialize()) {
            LOGE("Unable to initialize player");
        }

        BOOST_ASSERT(_dbusService);
        if (!_dbusService->start()) {
            LOGE("Unable to start Speaker DBus service");
        }

        BOOST_ASSERT(_httpService);
        if (!_httpService->start()) {
            LOGE("Unable to start Speaker HTTP service");
        }
    }

    void
    tearDown()
    {
        if (_httpService) {
            _httpService->stop();
        }

        if (_dbusService) {
            _dbusService->stop();
        }

        if (_player) {
            _player->finalize();
        }

        if (_observer) {
            _observer->remove(*_httpService);
            _observer->remove(*_dbusService);
        }

        if (_mqtt) {
            _mqtt->disconnect();
        }
    }

    void
    finalize()
    {
        _mqtt.reset();
        _publisher.reset();
        _observer.reset();
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
    std::unique_ptr<AvailabilityObserver> _observer;
    std::unique_ptr<Player> _player;
    std::unique_ptr<MqttPublisher> _mqtt;
    std::unique_ptr<AvailabilityPublisher> _publisher;
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
