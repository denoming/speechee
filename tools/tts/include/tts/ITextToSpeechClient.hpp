#pragma once

#include <string>
#include <system_error>

namespace jar {

class ITextToSpeechClient {
public:
    virtual ~ITextToSpeechClient() = default;

    /**
     * Synthesize given text into audio
     * @param text the text to synthesize
     * @param lang the language to use
     * @return the audio content bytes encoded as specified including the header for encoding
     * @throws std::runtime_error exception
     *
     */
    virtual std::string
    synthesizeText(const std::string& text, const std::string& lang)
        = 0;

    /**
     * Synthesize given SSML markup into audio
     * @param ssml the SSML markup to synthesize
     * @param lang the language to use
     * @return the audio content bytes encoded as specified including the header for encoding
     * @throws std::runtime_error exception
     */
    virtual std::string
    synthesizeSsml(const std::string& ssml, const std::string& lang)
        = 0;
};

} // namespace jar
