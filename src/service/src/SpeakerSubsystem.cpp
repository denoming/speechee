#include "speechee/SpeakerSubsystem.hpp"

#include "speechee/GstInitializer.hpp"
#include "speechee/Options.hpp"
#include "speechee/PlayerFactory.hpp"
#include "speechee/PlayerLoop.hpp"
#include "speechee/Speaker.hpp"
#include "speechee/SpeechSynthesizePool.hpp"

#ifdef ENABLE_ONLINE_TTS
#include "tts/OnlineTextToSpeechClient.hpp"
#endif
#ifdef ENABLE_OFFLINE_TTS
#include "tts/OfflineTextToSpeechEngine.hpp"
#endif

#ifdef ENABLE_DBUS_SUPPORT
#include "speechee/DbusSpeakerService.hpp"
#endif
#ifdef ENABLE_HTTP_SUPPORT
#include "speechee/HttpSpeakerService.hpp"
#endif

#include <jarvisto/core/Config.hpp>
#include <jarvisto/core/Logger.hpp>
#include <jarvisto/network/AvailabilityObserver.hpp>
#include <jarvisto/network/AvailabilityPublisher.hpp>
#include <jarvisto/network/MqttBasicClient.hpp>

#include <boost/assert.hpp>

static constexpr int64_t kDefaultSynthesisThreads{2};
static constexpr int64_t kDefaultServiceThreads{2};
static constexpr int64_t kDefaultServicePort{2350};

namespace jar {

class SpeakerSubsystem::Impl {
public:
    void
    initialize(Application& /*application*/)
    {
        _initializer = std::make_unique<GstInitializer>();
        if (not _initializer->initialize()) {
            LOGE("Unable to initialize gstreamer");
        }

        _config = std::make_unique<Config>();
        _config->load();

#ifdef ENABLE_ONLINE_TTS
        _client = std::make_unique<OnlineTextToSpeechClient>();
#endif
#ifdef ENABLE_OFFLINE_TTS
        std::optional<std::string> modelPath;
        if (const auto value = _config->pick<std::string>("synthesis.voiceModelPath")) {
            modelPath = *value;
        }
        std::optional<std::string> filesPath;
        if (const auto value = _config->pick<std::string>("synthesis.voiceFilesPath")) {
            filesPath = *value;
        }
        if (modelPath) {
            if (filesPath) {
                _client = std::make_unique<OfflineTextToSpeechEngine>(*modelPath, *filesPath);
            } else {
                _client = std::make_unique<OfflineTextToSpeechEngine>(*modelPath);
            }
        } else {
            LOGE("Paths to voice model must be set");
            throw std::runtime_error("Unable create offline speech engine");
        }
#endif

        int64_t synthesisThreads{kDefaultSynthesisThreads};
        if (const auto value = _config->pick<int64_t>("synthesis.threads")) {
            synthesisThreads = *value;
        }
        _synthesizePool = std::make_unique<SpeechSynthesizePool>(*_client, synthesisThreads);

        _playerLoop = std::make_unique<PlayerLoop>();
        _playerFactory = std::make_unique<PlayerFactory>(*_playerLoop);
        _speaker = std::make_unique<Speaker>(*_synthesizePool, *_playerFactory);
        _observer = std::make_unique<AvailabilityObserver>("speechee");
        _mqttClient = std::make_unique<MqttBasicClient>();
        _publisher = std::make_unique<AvailabilityPublisher>("speechee", *_mqttClient, *_observer);

#ifdef ENABLE_DBUS_SUPPORT
        _dbusService = std::make_unique<DbusSpeakerService>(*_speaker);
#endif
#ifdef ENABLE_HTTP_SUPPORT
        BOOST_ASSERT(_config);
        std::int64_t servicePort{kDefaultServicePort};
        if (const auto value = _config->pick<std::int64_t>("services.http.port")) {
            servicePort = *value;
        }
        std::int64_t serviceTreads{kDefaultServiceThreads};
        if (const auto value = _config->pick<std::int64_t>("services.http.threads")) {
            serviceTreads = *value;
        }
        _httpService = std::make_unique<HttpSpeakerService>(serviceTreads, servicePort, *_speaker);
#endif
    }

    void
    setUp(Application& /*application*/)
    {
        BOOST_ASSERT(_config);

        std::optional<std::string> mqttUser;
        if (const auto value = _config->pick<std::string>("mqtt.user")) {
            mqttUser = *value;
        }
        std::optional<std::string> mqttPass;
        if (const auto value = _config->pick<std::string>("mqtt.password")) {
            mqttPass = *value;
        }

        if (mqttUser and mqttPass) {
            BOOST_ASSERT(_mqttClient);
            if (const auto ec = _mqttClient->credentials(*mqttUser, *mqttPass); ec) {
                LOGE("Unable to set MQTT credentials: {}", ec.message());
            }
        }

        std::string mqttServer{"localhost"};
        if (const auto value = _config->pick<std::string>("mqtt.server")) {
            mqttServer = *value;
        }
        if (const auto ec = _mqttClient->connectAsync(mqttServer); ec) {
            LOGE("Unable to connect to <{}> MQTT broker: {}", mqttServer, ec.message());
        }

        BOOST_ASSERT(_playerLoop);
        _playerLoop->start();

#ifdef ENABLE_DBUS_SUPPORT
        BOOST_ASSERT(_observer);
        _observer->add(*_dbusService);
        BOOST_ASSERT(_dbusService);
        if (not _dbusService->start()) {
            LOGE("Unable to start Speaker DBus service");
        }
#endif

#ifdef ENABLE_HTTP_SUPPORT
        BOOST_ASSERT(_observer);
        _observer->add(*_httpService);
        BOOST_ASSERT(_httpService);
        if (not _httpService->start()) {
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

        if (_playerLoop) {
            _playerLoop->stop();
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
        _playerFactory.reset();
        _playerLoop.reset();
        _synthesizePool.reset();
        _client.reset();
        _config.reset();
    }

private:
    std::unique_ptr<GstInitializer> _initializer;
    std::unique_ptr<Config> _config;
    std::unique_ptr<Speaker> _speaker;
    std::unique_ptr<SpeechSynthesizePool> _synthesizePool;
    std::unique_ptr<ITextToSpeechClient> _client;
    std::unique_ptr<AvailabilityObserver> _observer;
    std::unique_ptr<PlayerLoop> _playerLoop;
    std::unique_ptr<PlayerFactory> _playerFactory;
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
