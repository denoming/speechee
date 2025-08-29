// Copyright 2025 Denys Asauliak
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
