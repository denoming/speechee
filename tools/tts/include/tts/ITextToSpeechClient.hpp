#pragma once

#include <string>
#include <system_error>
#include <memory>

namespace jar {

class ITextToSpeechClient {
public:
    using Ptr = std::unique_ptr<ITextToSpeechClient>;

    virtual ~ITextToSpeechClient() = default;

    virtual std::string
    synthesizeText(const std::string& text, const std::string& lang, std::error_code& error)
        = 0;

    virtual std::string
    synthesizeSsml(const std::string& ssml, const std::string& lang, std::error_code& error)
        = 0;
};

} // namespace jar
