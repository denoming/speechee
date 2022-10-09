#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "speech/Speaker.hpp"
#include "speech/SpeechSynthesizePool.hpp"
#include "speech/Player.hpp"
#include "speech/TextToSpeechClient.hpp"

#include <thread>

using namespace testing;
using namespace jar;

TEST(SpeakerTest, SynthesizeText)
{
    TextToSpeechClient client;
    SpeechSynthesizePool pool{client, 2};

    Player player;
    ASSERT_TRUE(player.initialize());
    std::this_thread::sleep_for(std::chrono::seconds{1});

    Speaker speaker{pool, player};
    speaker.synthesizeText("Hello my beautiful world", "en-gb");
    std::this_thread::sleep_for(std::chrono::seconds{30});

    player.finalize();
}