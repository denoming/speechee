#pragma once

#include "tts/ITextToSpeechClient.hpp"

#include <gmock/gmock.h>

namespace jar {

class MockTextToSpeechClient : public ITextToSpeechClient {
public:
    MOCK_METHOD(std::string,
                synthesizeText,
                (const std::string& text, const Voice& voice, const AudioConfig& audioConfig),
                (override));

    MOCK_METHOD(std::string,
                synthesizeSsml,
                (const std::string& ssml, const Voice& voice, const AudioConfig& audioConfig),
                (override));
};

} // namespace jar
