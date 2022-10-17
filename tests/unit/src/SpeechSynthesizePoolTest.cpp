#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "speech/SpeechSynthesizePool.hpp"
#include "tests/MockTextToSpeechClient.hpp"
#include "tests/Waiter.hpp"

#include <chrono>

using namespace testing;
using namespace jar;

class SpeechSynthesizePoolTest : public Test {
public:
    const std::chrono::seconds kDefaultTimeout{1};

    SpeechSynthesizePoolTest()
        : pool{client, std::thread::hardware_concurrency()}
    {
    }

public:
    NiceMock<MockTextToSpeechClient> client;
    SpeechSynthesizePool pool;
};

TEST_F(SpeechSynthesizePoolTest, SynthesizeText)
{
    std::string text{"World"};
    std::string lang{"uk-UA"};
    std::string cont{"audio"};

    EXPECT_CALL(client, synthesizeText(text, lang, _)).WillOnce(Return(cont));

    MockFunction<SpeechSynthesizePool::OnCompleteSig> callback;
    EXPECT_CALL(callback, Call(cont, std::error_code{}));

    Waiter<SpeechSynthesizePool::OnCompleteSig> waiter;
    pool.synthesizeText(
        text, lang, waiter.enroll([&](std::string audio, std::error_code errorCode) {
            callback.Call(std::move(audio), errorCode);
        }));

    EXPECT_TRUE(waiter.wait(kDefaultTimeout));
}

TEST_F(SpeechSynthesizePoolTest, SynthesizeSsml)
{
    std::string ssml{"World"};
    std::string lang{"uk-UA"};
    std::string cont{"audio"};

    EXPECT_CALL(client, synthesizeSsml(ssml, lang, _)).WillOnce(Return(cont));

    MockFunction<SpeechSynthesizePool::OnCompleteSig> callback;
    EXPECT_CALL(callback, Call(cont, std::error_code{}));

    Waiter<SpeechSynthesizePool::OnCompleteSig> waiter;
    pool.synthesizeSsml(
        ssml, lang, waiter.enroll([&](std::string audio, std::error_code errorCode) {
            callback.Call(std::move(audio), errorCode);
        }));

    EXPECT_TRUE(waiter.wait(kDefaultTimeout));
}
