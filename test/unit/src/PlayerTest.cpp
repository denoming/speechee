#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "speechee/GstInitializer.hpp"
#include "speechee/Player.hpp"
#include "speechee/PlayerLoop.hpp"
#include "test/Waiter.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>

using namespace testing;
using namespace jar;

namespace fs = std::filesystem;

static const fs::path AssetAudioFile1{fs::current_path() / "asset" / "audio" / "audio1.wav"};
static const fs::path AssetAudioFile2{fs::current_path() / "asset" / "audio" / "audio2.wav"};

static std::optional<std::string>
readAudioFile(const fs::path& filePath)
{
    if (!fs::exists(filePath)) {
        return std::nullopt;
    }
    const auto size = static_cast<std::streamsize>(fs::file_size(filePath));
    if (!size) {
        return std::nullopt;
    }

    std::ifstream stream{filePath};
    if (!stream) {
        return std::nullopt;
    }
    std::string audio;
    audio.resize(size, '\0');
    stream.read(&audio[0], size);
    return audio;
}

class PlayerTest : public Test {
public:
    const std::chrono::seconds kDefaultTimeout{15};

    static void
    SetUpTestSuite()
    {
        ASSERT_TRUE(initializer.initialize());

        auto audioOpt1 = readAudioFile(AssetAudioFile1);
        ASSERT_THAT(audioOpt1, Optional(Not(IsEmpty())));
        Audio1.swap(audioOpt1.value());
        auto audioOpt2 = readAudioFile(AssetAudioFile2);
        ASSERT_THAT(audioOpt2, Optional(Not(IsEmpty())));
        Audio2.swap(audioOpt2.value());
    }

    PlayerTest()
        : player{playerLoop}
    {
    }

    void
    SetUp() override
    {
        playerLoop.start();
    }

    void
    TearDown() override
    {
        playerLoop.stop();
    }

public:
    static std::string Audio1;
    static std::string Audio2;
    static GstInitializer initializer;
    PlayerLoop playerLoop;
    Player player;
};

std::string PlayerTest::Audio1;
std::string PlayerTest::Audio2;
GstInitializer PlayerTest::initializer;

TEST_F(PlayerTest, LifeCycle)
{
    Waiter<IPlayer::OnStateUpdate> waiter;
    MockFunction<IPlayer::OnStateUpdate> stateCallback;
    auto enrollCallback = waiter.enroll([&](const auto state) { stateCallback.Call(state); });
    auto c = player.onStateUpdate().connect(enrollCallback);

    waiter.onCheck([](const auto state) { return (state == PlayState::Idle); });
    EXPECT_CALL(stateCallback, Call(PlayState::Idle)).RetiresOnSaturation();
    EXPECT_TRUE(player.start());
    EXPECT_TRUE(waiter.wait(kDefaultTimeout));

    ASSERT_EQ(player.state(), PlayState::Idle);

    waiter.onCheck([](const auto state) { return (state == PlayState::Null); });
    EXPECT_CALL(stateCallback, Call(PlayState::Null)).RetiresOnSaturation();
    player.stop();
    EXPECT_TRUE(waiter.wait(kDefaultTimeout));

    c.disconnect();
}

TEST_F(PlayerTest, PlayAudio)
{
    Waiter<IPlayer::OnStateUpdate> waiter;
    MockFunction<IPlayer::OnStateUpdate> stateCallback;
    auto enrollCallback = waiter.enroll([&](const auto state) { stateCallback.Call(state); });
    auto c = player.onStateUpdate().connect(enrollCallback);

    /* 1: Start player */
    waiter.onCheck([](const auto state) { return (state == PlayState::Idle); });
    EXPECT_CALL(stateCallback, Call(PlayState::Idle)).RetiresOnSaturation();
    ASSERT_TRUE(player.start());
    EXPECT_TRUE(waiter.wait(kDefaultTimeout));

    /* 1: Waiting for playing Audio1 is finished */
    EXPECT_CALL(stateCallback, Call(PlayState::Busy)).Times(1).RetiresOnSaturation();
    EXPECT_CALL(stateCallback, Call(PlayState::Idle)).Times(1).RetiresOnSaturation();
    player.play(Audio1);
    EXPECT_TRUE(waiter.wait(kDefaultTimeout));

    /* 1: Stop player */
    EXPECT_CALL(stateCallback, Call(PlayState::Null)).RetiresOnSaturation();
    player.stop();

    /* 2: Start player */
    waiter.onCheck([](const auto state) { return (state == PlayState::Idle); });
    EXPECT_CALL(stateCallback, Call(PlayState::Idle)).RetiresOnSaturation();
    ASSERT_TRUE(player.start());
    EXPECT_TRUE(waiter.wait(kDefaultTimeout));

    /* 2: Waiting for playing Audio1 is finished */
    EXPECT_CALL(stateCallback, Call(PlayState::Busy)).Times(1).RetiresOnSaturation();
    EXPECT_CALL(stateCallback, Call(PlayState::Idle)).Times(1).RetiresOnSaturation();
    player.play(Audio2);
    EXPECT_TRUE(waiter.wait(kDefaultTimeout));

    /* 2: Stop player */
    EXPECT_CALL(stateCallback, Call(PlayState::Null)).RetiresOnSaturation();
    player.stop();

    c.disconnect();
}