#pragma once

#include "ITextToSpeechClient.hpp"

#include <memory>

namespace jar {

class TextToSpeechClient final : public ITextToSpeechClient {
public:
    TextToSpeechClient();

    std::string
    synthesizeText(const std::string& text, const std::string& lang, std::error_code& error) final;

    std::string
    synthesizeSsml(const std::string& text, const std::string& lang, std::error_code& error) final;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar