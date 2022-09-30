#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "speech/SpeechSynthesizePool.hpp"
#include "tests/MockTextToSpeechClient.hpp"
#include "tests/TestWaiter.hpp"

#include <chrono>

using namespace testing;
using namespace jar;

class SpeechSynthesizePoolTest : public Test {
public:
    const std::chrono::seconds kTimeout{1};

    SpeechSynthesizePoolTest()
        : pool{client, std::thread::hardware_concurrency()}
    {
    }

public:
    NiceMock<MockTextToSpeechClient> client;
    SpeechSynthesizePool pool;
    TestWaiter waiter;
};

TEST_F(SpeechSynthesizePoolTest, SynthesizeText)
{
    std::string text{"World"};
    std::string lang{"uk-UA"};
    std::string cont{"audio"};

    EXPECT_CALL(client, synthesizeText(text, lang, _)).WillOnce(Return(cont));

    MockFunction<SpeechSynthesizePool::OnCompleteSig> callback;
    EXPECT_CALL(callback, Call(cont, std::error_code{}));

    bool guard;
    pool.synthesizeText(text, lang, [&](std::string audio, std::error_code error) {
        guard = true;
        callback.Call(std::move(audio), error);
        waiter.notify();
    });

    ASSERT_TRUE(waiter.waitFor(kTimeout, [&]() { return guard; }));
}

TEST_F(SpeechSynthesizePoolTest, SynthesizeSsml)
{
    std::string ssml{"World"};
    std::string lang{"uk-UA"};
    std::string cont{"audio"};

    EXPECT_CALL(client, synthesizeSsml(ssml, lang, _)).WillOnce(Return(cont));

    MockFunction<SpeechSynthesizePool::OnCompleteSig> callback;
    EXPECT_CALL(callback, Call(cont, std::error_code{}));

    bool guard;
    pool.synthesizeSsml(ssml, lang, [&](std::string audio, std::error_code error) {
        guard = true;
        callback.Call(std::move(audio), error);
        waiter.notify();
    });

    ASSERT_TRUE(waiter.waitFor(kTimeout, [&]() { return guard; }));
}
