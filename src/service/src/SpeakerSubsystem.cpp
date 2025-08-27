#include "speechee/SpeakerSubsystem.hpp"

#include "speechee/Constants.hpp"
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
#include <jarvisto/network/MqttClient.hpp>

#include <gsl/gsl-lite.hpp>

namespace jar {

class SpeakerSubsystem::Impl {
public:
    void
    initialize(Application& /*application*/)
    {
        _config = std::make_unique<Config>();
        _config->load();

        createAvailabilityClient();
        createTextToSpeechClient();
        createSpeaker();
        createSpeakerServices();
    }

    void
    setUp(Application& /*application*/)
    {
        connectAvailabilityClient();

        gsl_Assert(_playerLoop);
        _playerLoop->start();

        startSpeakerServices();
    }

    void
    tearDown()
    {
        stopSpeakerServices();

        if (_playerLoop) {
            _playerLoop->stop();
        }

        disconnectAvailabilityClient();
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
    void
    createAvailabilityClient()
    {
        _observer = std::make_unique<AvailabilityObserver>("speechee");
        _mqttClient = std::make_unique<MqttClient>();
        _publisher = std::make_unique<AvailabilityPublisher>("speechee", *_mqttClient, *_observer);
    }

    void
    createTextToSpeechClient()
    {
#ifdef ENABLE_ONLINE_TTS
        _client = std::make_unique<OnlineTextToSpeechClient>();
#endif
#ifdef ENABLE_OFFLINE_TTS
        std::optional<std::string> modelPath;
        if (const auto value = _config->pick<std::string>("synthesis.modelFile")) {
            modelPath = *value;
        }
        std::optional<std::string> filesPath;
        if (const auto value = _config->pick<std::string>("synthesis.espeakDir")) {
            filesPath = *value;
        }
        if (modelPath) {
            gsl_Assert(not _client);
            if (filesPath) {
                LOGD("Create offline TTS using <{}> model and <{}> eSpeak dir",
                     *modelPath,
                     *filesPath);
                _client = std::make_unique<OfflineTextToSpeechEngine>(*modelPath, *filesPath);
            } else {
                LOGD("Create offline TTS using <{}> model");
                _client = std::make_unique<OfflineTextToSpeechEngine>(*modelPath);
            }
        } else {
            LOGE("Paths to voice model must be set");
            throw std::runtime_error("Unable create offline speech engine");
        }
#endif
    }

    void
    createSpeaker()
    {
        if (_initializer = std::make_unique<GstInitializer>(); not _initializer->initialize()) {
            throw std::runtime_error("Unable to initialize gstreamer");
        }

        int64_t synthesisThreads{kSynthesisPoolThreads};
        gsl_Assert(_config);
        if (const auto value = _config->pick<int64_t>("synthesis.threads")) {
            synthesisThreads = *value;
        }
        LOGD("Create synthesize pool with <{}> threads", synthesisThreads);
        _synthesizePool = std::make_unique<SpeechSynthesizePool>(*_client, synthesisThreads);

        _playerLoop = std::make_unique<PlayerLoop>();
        _playerFactory = std::make_unique<PlayerFactory>(*_playerLoop);

        LOGD("Create speaker");
        _speaker = std::make_unique<Speaker>(*_synthesizePool, *_playerFactory);
    }

    void
    createSpeakerServices()
    {
#ifdef ENABLE_DBUS_SUPPORT
        LOGD("Create dbus service");
        _dbusService = std::make_unique<DbusSpeakerService>(*_speaker);
#endif

#ifdef ENABLE_HTTP_SUPPORT
        gsl_Assert(_config);
        std::int64_t servicePort{kHttpServiceThreads};
        if (const auto value = _config->pick<std::int64_t>("services.http.port")) {
            servicePort = *value;
        }
        std::int64_t serviceTreads{kHttpServiceThreads};
        if (const auto value = _config->pick<std::int64_t>("services.http.threads")) {
            serviceTreads = *value;
        }
        LOGD("Create HTTP service on <{}> port with <{}> threads", servicePort, serviceTreads);
        _httpService = std::make_unique<HttpSpeakerService>(serviceTreads, servicePort, *_speaker);
#endif
    }

    void
    startSpeakerServices() const
    {
#ifdef ENABLE_DBUS_SUPPORT
        LOGD("Start dbus service");
        gsl_Assert(_observer);
        _observer->add(*_dbusService);
        gsl_Assert(_dbusService);
        if (not _dbusService->start()) {
            LOGE("Unable to start Speaker DBus service");
        }
#endif

#ifdef ENABLE_HTTP_SUPPORT
        LOGD("Start HTTP service");
        gsl_Assert(_observer);
        _observer->add(*_httpService);
        gsl_Assert(_httpService);
        if (not _httpService->start()) {
            LOGE("Unable to start Speaker HTTP service");
        }
#endif
    }

    void
    stopSpeakerServices() const
    {
#ifdef ENABLE_DBUS_SUPPORT
        if (_dbusService) {
            LOGD("Stop dbus service");
            _dbusService->stop();
            if (_observer) {
                _observer->remove(*_dbusService);
            }
        }
#endif

#ifdef ENABLE_HTTP_SUPPORT
        if (_httpService) {
            LOGD("Stop HTTP service");
            _httpService->stop();
            if (_observer) {
                _observer->remove(*_httpService);
            }
        }
#endif
    }

    void
    connectAvailabilityClient() const
    {
        gsl_Assert(_config);
        std::optional<std::string> mqttUser;
        if (const auto value = _config->pick<std::string>("mqtt.user")) {
            mqttUser = *value;
        }
        std::optional<std::string> mqttPass;
        if (const auto value = _config->pick<std::string>("mqtt.password")) {
            mqttPass = *value;
        }
        std::string mqttServer{"localhost"};
        if (const auto value = _config->pick<std::string>("mqtt.server")) {
            mqttServer = *value;
        }

        gsl_Assert(_mqttClient);
        if (mqttUser and mqttPass) {
            if (const auto ec = _mqttClient->credentials(*mqttUser, *mqttPass); ec) {
                LOGE("Unable to set MQTT credentials: {}", ec.message());
            }
        }
        LOGD("Connect to <{}> server", mqttServer);
        if (const auto ec = _mqttClient->connectAsync(mqttServer); ec) {
            LOGE("Unable to connect to <{}> MQTT broker: {}", mqttServer, ec.message());
        }
    }

    void
    disconnectAvailabilityClient() const
    {
        if (_mqttClient) {
            std::ignore = _mqttClient->disconnect();
        }
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
    std::unique_ptr<MqttClient> _mqttClient;
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

    gsl_Assert(_impl);
    _impl->initialize(application);
}

void
SpeakerSubsystem::setUp(Application& application)
{
    Subsystem::setUp(application);

    gsl_Assert(_impl);
    _impl->setUp(application);
}

void
SpeakerSubsystem::tearDown()
{
    Subsystem::tearDown();

    gsl_Assert(_impl);
    _impl->tearDown();
}

void
SpeakerSubsystem::finalize()
{
    Subsystem::finalize();

    gsl_Assert(_impl);
    _impl->finalize();
}

} // namespace jar
