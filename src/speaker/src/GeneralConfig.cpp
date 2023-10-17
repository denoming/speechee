#include "speaker/GeneralConfig.hpp"

#include <jarvisto/Logger.hpp>
#include <jarvisto/Utils.hpp>

namespace fs = std::filesystem;

namespace jar {

size_t
GeneralConfig::synthesisThreads() const
{
    return _synthesisThreads;
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
        config.lookupValue("synthesis.threads", _synthesisThreads);
        config.lookupValue("mqtt.user", _mqttUser);
        config.lookupValue("mqtt.password", _mqttPassword);
        config.lookupValue("mqtt.server", _mqttServer);
        config.lookupValue("mqtt.server", _mqttServer);
        config.lookupValue("services.http.port", _httpServicePort);
        config.lookupValue("services.http.threads", _httpServiceThreads);
    } catch (...) {
        // Suppress any exceptions
    }
    return true;
}

} // namespace jar
