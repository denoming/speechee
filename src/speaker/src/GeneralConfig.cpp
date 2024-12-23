#include "speaker/GeneralConfig.hpp"

#include <jarvisto/core/Logger.hpp>

namespace fs = std::filesystem;

static const auto* kConfigPath1{"synthesis.threads"};
static const auto* kConfigPath2{"synthesis.voiceFilesPath"};
static const auto* kConfigPath3{"synthesis.voiceModelPath"};
static const auto* kConfigPath4{"mqtt.user"};
static const auto* kConfigPath5{"mqtt.password"};
static const auto* kConfigPath6{"mqtt.server"};
static const auto* kConfigPath7{"services.http.port"};
static const auto* kConfigPath8{"services.http.threads"};

namespace jar {

size_t
GeneralConfig::synthesisThreads() const
{
    return _synthesisThreads;
}

const std::optional<std::string>&
GeneralConfig::synthesisVoiceFilesPath() const
{
    return _synthesisVoiceFilesPath;
}

const std::optional<std::string>&
GeneralConfig::synthesisVoiceModelPath() const
{
    return _synthesisVoiceModelPath;
}

const std::string&
GeneralConfig::mqttUser() const
{
    return _mqttUser;
}

const std::string&
GeneralConfig::mqttPassword() const
{
    return _mqttPassword;
}

const std::string&
GeneralConfig::mqttServer() const
{
    return _mqttServer;
}

uint16_t
GeneralConfig::httpServicePort() const
{
    return _httpServicePort;
}

size_t
GeneralConfig::httpServiceThreads() const
{
    return _httpServiceThreads;
}

bool
GeneralConfig::doParse(const libconfig::Config& config)
{
    try {
        config.lookupValue(kConfigPath1, _synthesisThreads);
        if (config.exists(kConfigPath2)) {
            if (std::string value; config.lookupValue(kConfigPath2, value)) {
                _synthesisVoiceFilesPath = std::move(value);
            }
        }
        if (config.exists(kConfigPath3)) {
            if (std::string value; config.lookupValue(kConfigPath3, value)) {
                _synthesisVoiceModelPath = std::move(value);
            }
        }
        config.lookupValue(kConfigPath4, _mqttUser);
        config.lookupValue(kConfigPath5, _mqttPassword);
        config.lookupValue(kConfigPath6, _mqttServer);
        config.lookupValue(kConfigPath7, _httpServicePort);
        config.lookupValue(kConfigPath8, _httpServiceThreads);
    } catch (...) {
        // Suppress any exceptions
    }
    return true;
}

} // namespace jar
