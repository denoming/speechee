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

#pragma once

#include <functional>
#include <string_view>
#include <exception>

namespace jar {

class ISpeechSynthesizePool {
public:
    using OnDone = void(std::string audio, std::exception_ptr exception);

    virtual ~ISpeechSynthesizePool() = default;

    virtual void
    synthesizeText(std::string_view text,
                   std::string_view lang,
                   std::move_only_function<OnDone> callback)
        = 0;

    virtual void
    synthesizeSsml(std::string_view ssml,
                   std::string_view lang,
                   std::move_only_function<OnDone> callback)
        = 0;
};

} // namespace jar
