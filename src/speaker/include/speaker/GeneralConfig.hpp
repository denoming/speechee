#pragma once

#include "speaker/ConfigReader.hpp"

#include <string>
#include <filesystem>

namespace jar {

class GeneralConfig final : private ConfigReader {
public:
    static inline uint32_t kSynthesisThreads = 2;
    static inline uint32_t kHttpServicePort = 8080;
    static inline uint32_t kHttpServiceThreads = 2;

    GeneralConfig() = default;

    [[nodiscard]] bool
    load();

    [[nodiscard]] bool
    load(const std::filesystem::path& path);

    [[nodiscard]] bool
    load(std::string_view str);

    [[nodiscard]] size_t
    synthesisThreads() const;

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
    std::string _mqttUser;
    std::string _mqttPassword;
    std::string _mqttServer;
    uint32_t _httpServicePort{kHttpServicePort};
    uint32_t _httpServiceThreads{kHttpServiceThreads};
};

} // namespace jar