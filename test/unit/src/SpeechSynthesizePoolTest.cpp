#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "speaker/SpeechSynthesizePool.hpp"
#include "test/MockTextToSpeechClient.hpp"
#include "test/Waiter.hpp"

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

    EXPECT_CALL(client, synthesizeText(text, lang)).WillOnce(Return(cont));

    MockFunction<SpeechSynthesizePool::OnDone> callback;
    EXPECT_CALL(callback, Call(cont, std::exception_ptr{}));

    Waiter<SpeechSynthesizePool::OnDone> waiter;
    pool.synthesizeText(
        text, lang, waiter.enroll([&](std::string audio, std::exception_ptr exception) {
            callback.Call(std::move(audio), std::move(exception));
        }));

    EXPECT_TRUE(waiter.wait(kDefaultTimeout));
}

TEST_F(SpeechSynthesizePoolTest, SynthesizeSsml)
{
    std::string ssml{"World"};
    std::string lang{"uk-UA"};
    std::string cont{"audio"};

    EXPECT_CALL(client, synthesizeSsml(ssml, lang)).WillOnce(Return(cont));

    MockFunction<SpeechSynthesizePool::OnDone> callback;
    EXPECT_CALL(callback, Call(cont, std::exception_ptr{}));

    Waiter<SpeechSynthesizePool::OnDone> waiter;
    pool.synthesizeSsml(
        ssml, lang, waiter.enroll([&](std::string audio, std::exception_ptr exception) {
            callback.Call(std::move(audio), std::move(exception));
        }));

    EXPECT_TRUE(waiter.wait(kDefaultTimeout));
}
