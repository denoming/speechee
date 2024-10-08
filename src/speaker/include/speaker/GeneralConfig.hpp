#pragma once

#include "speaker/ConfigLoader.hpp"

#include <filesystem>
#include <optional>
#include <string>

namespace jar {

class GeneralConfig final : public ConfigLoader {
public:
    static inline uint32_t kSynthesisThreads = 2;
    static inline uint32_t kHttpServicePort = 8080;
    static inline uint32_t kHttpServiceThreads = 2;

    GeneralConfig() = default;

    [[nodiscard]] size_t
    synthesisThreads() const;

    [[nodiscard]] const std::optional<std::string>&
    synthesisVoiceFilesPath() const;

    [[nodiscard]] const std::optional<std::string>&
    synthesisVoiceModelPath() const;

    [[nodiscard]] const std::string&
    mqttUser() const;

    [[nodiscard]] const std::string&
    mqttPassword() const;

    [[nodiscard]] const std::string&
    mqttServer() const;

    [[nodiscard]] uint16_t
    httpServicePort() const;

    [[nodiscard]] size_t
    httpServiceThreads() const;

private:
    bool
    doParse(const libconfig::Config& config) final;

private:
    uint32_t _synthesisThreads{kSynthesisThreads};
    std::optional<std::string> _synthesisVoiceFilesPath;
    std::optional<std::string> _synthesisVoiceModelPath;
    std::string _mqttUser;
    std::string _mqttPassword;
    std::string _mqttServer;
    uint32_t _httpServicePort{kHttpServicePort};
    uint32_t _httpServiceThreads{kHttpServiceThreads};
};

} // namespace jar