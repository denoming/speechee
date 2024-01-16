#pragma once

#include "tts/Types.h"

#include <string>
#include <system_error>

namespace jar {

/**
 * Cloud text-to-speech C++ client wrapper
 * (see https://cloud.google.com/text-to-speech/docs/reference/rpc/google.cloud.texttospeech.v1)
 */
class ITextToSpeechClient {
public:
    virtual ~ITextToSpeechClient() = default;

    /**
     * Synthesize given text into audio
     * @param text the text to synthesize
     * @param voice the voice configuration
     * @param audioConfig the audio config
     * @return the audio content bytes encoded as specified including the header for encoding
     * @throws std::runtime_error exception
     */
    virtual std::string
    synthesizeText(const std::string& text, const Voice& voice, const AudioConfig& audioConfig)
        = 0;

    /**
     * Synthesize given SSML markup into audio
     * @param ssml the SSML markup to synthesize
     * @param voice the voice configuration
     * @param audioConfig the audio config
     * @return the audio content bytes encoded as specified including the header for encoding
     * @throws std::runtime_error exception
     */
    virtual std::string
    synthesizeSsml(const std::string& ssml, const Voice& voice, const AudioConfig& audioConfig)
        = 0;
};

} // namespace jar
