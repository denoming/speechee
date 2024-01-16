#pragma once

#include "tts/ITextToSpeechClient.hpp"

#include <memory>

namespace jar {

class TextToSpeechClient final : public ITextToSpeechClient {
public:
    TextToSpeechClient();

    ~TextToSpeechClient() final;

    std::string
    synthesizeText(const std::string& text);

    std::string
    synthesizeText(const std::string& text,
                   const Voice& voice,
                   const AudioConfig& audioConfig) final;

    std::string
    synthesizeSsml(const std::string& ssml);

    std::string
    synthesizeSsml(const std::string& ssml,
                   const Voice& voice,
                   const AudioConfig& audioConfig) final;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar
