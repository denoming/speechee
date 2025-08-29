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

#include "speechee/MockSpeechSynthesizePool.hpp"

using namespace testing;

namespace jar {

MockSpeechSynthesizePool::MockSpeechSynthesizePool()
{
    ON_CALL(*this, synthesizeText)
        .WillByDefault([](std::string_view text,
                          std::string_view lang,
                          std::move_only_function<OnDone> callback) {
            callback(std::string{}, std::make_exception_ptr(std::runtime_error{"Invalid argument"}));
        });

    ON_CALL(*this, synthesizeSsml)
        .WillByDefault([](std::string_view ssml,
                          std::string_view lang,
                          std::move_only_function<OnDone> callback) {
            callback(std::string{}, std::make_exception_ptr(std::runtime_error{"Invalid argument"}));
        });
}

} // namespace jar
