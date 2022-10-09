#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "speech/Player.hpp"
#include "tests/TestWaiter.hpp"

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
    TestWaiter waiter;
    Player player;
};

std::string PlayerTest::Audio1;
std::string PlayerTest::Audio2;

TEST_F(PlayerTest, LifeCycle)
{
    EXPECT_EQ(player.state(), PlayState::Null);

    MockFunction<IPlayer::OnStateUpdate> callback;
    auto c = player.onStateUpdate([&](const PlayState s) { callback.Call(s); });

    EXPECT_CALL(callback, Call(PlayState::Idle)).RetiresOnSaturation();
    EXPECT_TRUE(player.initialize());

    EXPECT_CALL(callback, Call(PlayState::Null)).RetiresOnSaturation();
    player.finalize();

    c.disconnect();
}

TEST_F(PlayerTest, Play)
{
    using namespace std::chrono_literals;

    MockFunction<IPlayer::OnStateUpdate> callback;
    auto c = player.onStateUpdate([&](PlayState s) { callback.Call(s); });

    EXPECT_CALL(callback, Call(PlayState::Idle)).RetiresOnSaturation();
    ASSERT_TRUE(player.initialize());

    bool guard{false};
    EXPECT_CALL(callback, Call(PlayState::Busy)).Times(2);
    EXPECT_CALL(callback, Call(PlayState::Idle)).Times(2).WillRepeatedly([&]() {
        /* Notify about playing is finished */
        guard = true;
        waiter.notify();
    });

    /* Waiting for playing Audio1 is finished */
    player.play(Audio1);
    ASSERT_TRUE(waiter.waitFor(30s, [&]() { return guard; }));

    EXPECT_EQ(player.state(), PlayState::Idle);
    guard = false;

    /* Waiting for playing Audio2 is finished */
    player.play(Audio2);
    ASSERT_TRUE(waiter.waitFor(30s, [&]() { return guard; }));

    EXPECT_CALL(callback, Call(PlayState::Null)).RetiresOnSaturation();
    player.finalize();

    c.disconnect();
}