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

#include "speechee/MockTextToSpeechClient.hpp"
#include "speechee/SpeechSynthesizePool.hpp"
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

    EXPECT_CALL(client, synthesizeText(text, _, _)).WillOnce(Return(cont));

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

    EXPECT_CALL(client, synthesizeSsml(ssml, _, _)).WillOnce(Return(cont));

    MockFunction<SpeechSynthesizePool::OnDone> callback;
    EXPECT_CALL(callback, Call(cont, std::exception_ptr{}));

    Waiter<SpeechSynthesizePool::OnDone> waiter;
    pool.synthesizeSsml(
        ssml, lang, waiter.enroll([&](std::string audio, std::exception_ptr exception) {
            callback.Call(std::move(audio), std::move(exception));
        }));

    EXPECT_TRUE(waiter.wait(kDefaultTimeout));
}
