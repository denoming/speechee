#pragma once

#include "speechee/ISpeechSynthesizePool.hpp"

#include <gmock/gmock.h>

namespace jar {

class MockSpeechSynthesizePool : public ISpeechSynthesizePool {
public:
    MockSpeechSynthesizePool();

    MOCK_METHOD(void,
                synthesizeText,
                (std::string_view text, std::string_view lang, std::move_only_function<OnDone>),
                (override));

    MOCK_METHOD(void,
                synthesizeSsml,
                (std::string_view ssml, std::string_view lang, std::move_only_function<OnDone>),
                (override));
};

} // namespace jar
