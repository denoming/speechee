#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "speech/Player.hpp"
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
        auto audioOpt1 = readAudioFile(AssetAudioFile1);
        ASSERT_THAT(audioOpt1, Optional(Not(IsEmpty())));
        Audio1.swap(audioOpt1.value());
        auto audioOpt2 = readAudioFile(AssetAudioFile2);
        ASSERT_THAT(audioOpt2, Optional(Not(IsEmpty())));
        Audio2.swap(audioOpt2.value());
    }

public:
    static std::string Audio1;
    static std::string Audio2;
    Player player;
};

std::string PlayerTest::Audio1;
std::string PlayerTest::Audio2;

TEST_F(PlayerTest, LifeCycle)
{
    MockFunction<IPlayer::OnStateUpdate> callback;

    ASSERT_EQ(player.state(), PlayState::Null);

    Waiter<IPlayer::OnStateUpdate> waiter;
    auto c = player.onStateUpdate(waiter.enroll([&](const auto state) { callback.Call(state); }));

    waiter.onCheck([](const auto state) { return (state == PlayState::Idle); });
    EXPECT_CALL(callback, Call(PlayState::Idle)).RetiresOnSaturation();
    EXPECT_TRUE(player.initialize());
    EXPECT_TRUE(waiter.wait(kDefaultTimeout));

    ASSERT_EQ(player.state(), PlayState::Idle);

    waiter.onCheck([](const auto state) { return (state == PlayState::Null); });
    EXPECT_CALL(callback, Call(PlayState::Null)).RetiresOnSaturation();
    player.finalize();
    EXPECT_TRUE(waiter.wait(kDefaultTimeout));

    c.disconnect();
}

TEST_F(PlayerTest, Play)
{
    MockFunction<IPlayer::OnStateUpdate> callback;

    Waiter<IPlayer::OnStateUpdate> waiter;
    auto c = player.onStateUpdate(waiter.enroll([&](const auto state) { callback.Call(state); }));

    waiter.onCheck([](const auto state) { return (state == PlayState::Idle); });
    EXPECT_CALL(callback, Call(PlayState::Idle)).RetiresOnSaturation();
    ASSERT_TRUE(player.initialize());
    EXPECT_TRUE(waiter.wait(kDefaultTimeout));

    EXPECT_CALL(callback, Call(PlayState::Busy)).Times(2).RetiresOnSaturation();
    EXPECT_CALL(callback, Call(PlayState::Idle)).Times(2).RetiresOnSaturation();

    /* Waiting for playing Audio1 is finished */
    player.play(Audio1);
    EXPECT_TRUE(waiter.wait(kDefaultTimeout));
    ASSERT_EQ(player.state(), PlayState::Idle);

    /* Waiting for playing Audio2 is finished */
    player.play(Audio2);
    ASSERT_TRUE(waiter.wait(kDefaultTimeout));
    ASSERT_EQ(player.state(), PlayState::Idle);

    EXPECT_CALL(callback, Call(PlayState::Null)).RetiresOnSaturation();
    player.finalize();

    c.disconnect();
}