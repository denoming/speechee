#include "speaker/SpeakerSubsystem.hpp"

#include "speaker/GeneralConfig.hpp"
#include "speaker/Player.hpp"
#include "speaker/Speaker.hpp"
#include "speaker/SpeechSynthesizePool.hpp"
#include "speechee/Options.hpp"
#include "tts/TextToSpeechClient.hpp"

#ifdef ENABLE_DBUS_SUPPORT
#include "speaker/DbusSpeakerService.hpp"
#endif
#ifdef ENABLE_HTTP_SUPPORT
#include "speaker/HttpSpeakerService.hpp"
#endif

#include <jarvisto/AvailabilityObserver.hpp>
#include <jarvisto/AvailabilityPublisher.hpp>
#include <jarvisto/Logger.hpp>
#include <jarvisto/MqttBasicClient.hpp>

#include <boost/assert.hpp>

namespace jar {

class SpeakerSubsystem::Impl {
public:
    void
    initialize(Application& /*application*/)
    {
        _config = std::make_unique<GeneralConfig>();
        if (not _config->load()) {
            LOGE("Unable to load config file");
        }

        _client = std::make_unique<TextToSpeechClient>();
        _synthesizePool
            = std::make_unique<SpeechSynthesizePool>(*_client, _config->synthesisThreads());
        _player = std::make_unique<Player>();
        _speaker = std::make_unique<Speaker>(*_synthesizePool, *_player);
        _observer = std::make_unique<AvailabilityObserver>("speechee");
        _mqttClient = std::make_unique<MqttBasicClient>();
        _publisher = std::make_unique<AvailabilityPublisher>("speechee", *_mqttClient, *_observer);
#ifdef ENABLE_DBUS_SUPPORT
        _dbusService = std::make_unique<DbusSpeakerService>(*_speaker);
#endif
#ifdef ENABLE_HTTP_SUPPORT
        BOOST_ASSERT(_config);
        _httpService = std::make_unique<HttpSpeakerService>(
            _config->httpServiceThreads(), _config->httpServicePort(), *_speaker);
#endif
    }

    void
    setUp(Application& /*application*/)
    {
        BOOST_ASSERT(_mqttClient);
        BOOST_ASSERT(_config);
        auto ec = _mqttClient->credentials(_config->mqttUser(), _config->mqttPassword());
        if (ec) {
            LOGE("Unable to set MQTT credentials: {}", ec.message());
        } else {
            ec = _mqttClient->connectAsync(_config->mqttServer());
            if (ec) {
                LOGE("Unable to initiate connection to MQTT broker: {}", ec.message());
            }
        }

        BOOST_ASSERT(_player);
        if (!_player->initialize()) {
            LOGE("Unable to initialize player");
        }

#ifdef ENABLE_DBUS_SUPPORT
        BOOST_ASSERT(_observer);
        _observer->add(*_dbusService);
        BOOST_ASSERT(_dbusService);
        if (!_dbusService->start()) {
            LOGE("Unable to start Speaker DBus service");
        }
#endif

#ifdef ENABLE_HTTP_SUPPORT
        BOOST_ASSERT(_observer);
        _observer->add(*_httpService);
        BOOST_ASSERT(_httpService);
        if (!_httpService->start()) {
            LOGE("Unable to start Speaker HTTP service");
        }
#endif
    }

    void
    tearDown()
    {
#ifdef ENABLE_DBUS_SUPPORT
        if (_dbusService) {
            _dbusService->stop();
        }
        if (_observer) {
            _observer->remove(*_dbusService);
        }

#endif

#ifdef ENABLE_HTTP_SUPPORT
        if (_httpService) {
            _httpService->stop();
        }
        if (_observer) {
            _observer->remove(*_httpService);
        }
#endif

        if (_player) {
            _player->finalize();
        }

        if (_mqttClient) {
            std::ignore = _mqttClient->disconnect();
        }
    }

    void
    finalize()
    {
        _mqttClient.reset();
        _publisher.reset();
        _observer.reset();
#ifdef ENABLE_DBUS_SUPPORT
        _dbusService.reset();
#endif
#ifdef ENABLE_HTTP_SUPPORT
        _httpService.reset();
#endif
        _speaker.reset();
        _player.reset();
        _synthesizePool.reset();
        _client.reset();
        _config.reset();
    }

private:
    std::unique_ptr<GeneralConfig> _config;
    std::unique_ptr<Speaker> _speaker;
    std::unique_ptr<SpeechSynthesizePool> _synthesizePool;
    std::unique_ptr<TextToSpeechClient> _client;
    std::unique_ptr<AvailabilityObserver> _observer;
    std::unique_ptr<Player> _player;
    std::unique_ptr<MqttBasicClient> _mqttClient;
    std::unique_ptr<AvailabilityPublisher> _publisher;
#ifdef ENABLE_DBUS_SUPPORT
    std::unique_ptr<DbusSpeakerService> _dbusService;
#endif
#ifdef ENABLE_HTTP_SUPPORT
    std::unique_ptr<HttpSpeakerService> _httpService;
#endif
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
