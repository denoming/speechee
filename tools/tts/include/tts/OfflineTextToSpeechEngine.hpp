#pragma once

#include "tts/ITextToSpeechClient.hpp"

#include <filesystem>
#include <memory>

namespace jar {

class OfflineTextToSpeechEngine final : public ITextToSpeechClient {
public:
    explicit OfflineTextToSpeechEngine(const std::filesystem::path& modelPath);

    OfflineTextToSpeechEngine(const std::filesystem::path& modelPath,
                              const std::filesystem::path& filesPath);

    ~OfflineTextToSpeechEngine() override;

    std::string
    synthesizeText(const std::string& text,
                   const Voice& voice,
                   const AudioConfig& audioConfig) override;

    std::string
    synthesizeSsml(const std::string& ssml,
                   const Voice& voice,
                   const AudioConfig& audioConfig) override;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar
