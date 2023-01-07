#pragma once

#include "speaker/ITextToSpeechClient.hpp"

#include <gmock/gmock.h>

namespace jar {

class MockTextToSpeechClient : public ITextToSpeechClient {
public:
    MOCK_METHOD(std::string,
                synthesizeText,
                (const std::string& text, const std::string& lang, std::error_code&),
                (override));

    MOCK_METHOD(std::string,
                synthesizeSsml,
                (const std::string& ssml, const std::string& lang, std::error_code&),
                (override));
};

} // namespace jar