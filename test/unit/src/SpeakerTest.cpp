// Copyright 2025 Denys Asauliak
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "speechee/MockPlayer.hpp"
#include "speechee/MockPlayerFactory.hpp"
#include "speechee/MockSpeechSynthesizePool.hpp"
#include "speechee/Speaker.hpp"
#include "speechee/SpeechSynthesizePool.hpp"

using namespace testing;
using namespace jar;

class SpeakerTest : public Test {
public:
    using Player = NiceMock<MockPlayer>;

    SpeakerTest()
        : speaker{pool, factory}
        , player{std::make_shared<Player>()}
    {
    }

    void
    SetUp() override
    {
        ON_CALL(factory, create).WillByDefault(Return(player));
    }

public:
    std::shared_ptr<Player> player;
    NiceMock<MockPlayerFactory> factory;
    NiceMock<MockSpeechSynthesizePool> pool;
    Speaker speaker;
};

TEST_F(SpeakerTest, SynthesizeText)
{
    static const std::string kAudio{"AUDIO1"};
    static constexpr std::string_view kText{"Some text"};
    static constexpr std::string_view kLang{"en-gb"};

    std::move_only_function<ISpeechSynthesizePool::OnDone> callback;
    EXPECT_CALL(pool, synthesizeText(kText, kLang, _))
        .WillOnce([&](auto, auto, auto c) { callback = std::move(c); })
        .RetiresOnSaturation();
    speaker.synthesizeText(kText, kLang);

    EXPECT_CALL(*player, play(kAudio)).RetiresOnSaturation(); // Expect kAudio1 to be played first
    callback(kAudio, std::exception_ptr{});

    player->triggerStateUpdate(PlayState::Idle);
}

TEST_F(SpeakerTest, SynthesizeSsml)
{
    static constexpr std::string_view kSsml{"Some ssml"};
    static constexpr std::string_view kLang{"en-gb"};

    EXPECT_CALL(pool, synthesizeSsml(kSsml, kLang, _))
        .WillOnce([&](auto, auto, auto callback) { callback(std::string{}, std::exception_ptr{}); })
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

    callback("", std::make_exception_ptr(std::invalid_argument("Invalid")));
}
