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

#include <vector>
#include <string>

namespace jar {

enum class SsmlVoiceGender {
    // Use any type of voices
    Unspecified,
    // Use a male voice
    Male,
    // Use a female voice
    Female
};

enum class AudioEncoding {
    // Uncompressed 16-bit signed little-endian samples (Linear PCM). Audio content returned with WAV header.
    Linear16,
    // MP3 audio at 32kbps
    MP3,
    // Opus encoded audio wrapped in an ogg container
    OggOpus,
    // 8-bit samples that complement 14-bit audio samples using G.711 PCMU/mu-law (+ WAV header)
    Mulaw,
    // 8-bit samples that complement 14-bit audio samples using G.711 PCMU/A-law (+ WAV header)
    Alaw
};

struct Voice {
    // The language and the region of the voice expressed as a BCP-47 language tag (e.g. en-US)
    std::string languageCode{"en-US"};
    // The name of the voice (if note set language code will be used)
    std::string name;
    // The preferred gender of the voice
    SsmlVoiceGender ssmlGender{SsmlVoiceGender::Unspecified};
};

struct AudioConfig {
    // The format of the audio byte stream
    AudioEncoding encoding{AudioEncoding::Linear16};
    // Speaking rate/speed, in the range [0.25, 4.0] (the 0.0 values defaults to 1.0)
    double speakingRate{0.0};
    // Speaking pitch, in the range [-20.0, 20.0]
    double pitch{0.0};
    // Volume gain in the range [-96.0, 16.0] (in dB) of the normal native volume (0.0 normal native signal amplitude)
    double volumeGainDb{0.0};
    // The synthesis sample rate (in hertz)
    int32_t sampleRateHz{0};
    // A list with identifiers to select particular profiles
    std::vector<std::string> effects;
};

} // namespace jar
