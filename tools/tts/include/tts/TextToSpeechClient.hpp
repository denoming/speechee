#pragma once

#include "tts/ITextToSpeechClient.hpp"

#include <memory>

namespace jar {

class TextToSpeechClient final : public ITextToSpeechClient {
public:
    TextToSpeechClient();

    ~TextToSpeechClient() final;

    std::string
    synthesizeText(const std::string& text, const std::string& lang) final;

    std::string
    synthesizeSsml(const std::string& text, const std::string& lang) final;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar
