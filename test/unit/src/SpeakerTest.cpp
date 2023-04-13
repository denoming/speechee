#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "speaker/Speaker.hpp"
#include "speaker/SpeechSynthesizePool.hpp"
#include "test/MockPlayer.hpp"
#include "test/MockSpeechSynthesizePool.hpp"

using namespace testing;
using namespace jar;

class SpeakerTest : public Test {
public:
    SpeakerTest()
        : speaker{pool, player}
    {
    }

public:
    NiceMock<MockPlayer> player;
    NiceMock<MockSpeechSynthesizePool> pool;
    Speaker speaker;
};

TEST_F(SpeakerTest, SynthesizeText)
{
    static const std::string kAudio1{"AUDIO1"};
    static const std::string kAudio2{"AUDIO2"};
    static constexpr std::string_view kText{"Some text"};
    static constexpr std::string_view kLang{"en-gb"};

    std::move_only_function<ISpeechSynthesizePool::OnDone> callback1;
    EXPECT_CALL(pool, synthesizeText(kText, kLang, _))
        .WillOnce([&](auto, auto, auto c) { callback1 = std::move(c); })
        .RetiresOnSaturation();
    speaker.synthesizeText(kText, kLang);

    std::move_only_function<ISpeechSynthesizePool::OnDone> callback2;
    EXPECT_CALL(pool, synthesizeText(kText, kLang, _))
        .WillOnce([&](auto, auto, auto c) { callback2 = std::move(c); })
        .RetiresOnSaturation();
    speaker.synthesizeText(kText, kLang);

    InSequence s;
    EXPECT_CALL(player, play(kAudio1)); // Expect kAudio1 to be played first
    EXPECT_CALL(player, play(kAudio2)); // Expect kAudio2 to be played after kAudio1

    /* Reverse order of callback invoking */
    callback2(kAudio2, std::error_code{}); // kAudio2 is ready first
    callback1(kAudio1, std::error_code{}); // kAudio1 is ready after kAudio2

    player.triggerStateUpdate(PlayState::Idle);
}

TEST_F(SpeakerTest, SynthesizeSsml)
{
    static constexpr std::string_view kSsml{"Some ssml"};
    static constexpr std::string_view kLang{"en-gb"};

    EXPECT_CALL(pool, synthesizeSsml(kSsml, kLang, _))
        .WillOnce([&](auto, auto, auto callback) { callback(std::string{}, std::error_code{}); })
        .RetiresOnSaturation();
    speaker.synthesizeSsml(kSsml, kLang);
}

TEST_F(SpeakerTest, SynthesizeError)
{
    static constexpr std::string_view kText{"Some ssml"};
    static constexpr std::string_view kLang{"en-gb"};

    std::move_only_function<ISpeechSynthesizePool::OnDone> callback;
    EXPECT_CALL(pool, synthesizeText(kText, kLang, _))
        .WillOnce([&](auto, auto, auto c) { callback = std::move(c); })
        .RetiresOnSaturation();

    speaker.synthesizeText(kText, kLang);

    callback("", std::make_error_code(std::errc::invalid_argument));
}
