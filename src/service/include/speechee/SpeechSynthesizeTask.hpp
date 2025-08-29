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
#include <string>
#include <string_view>
#include <exception>

namespace jar {

class ITextToSpeechClient;

class SpeechSynthesizeTask {
public:
    using OnDone = void(std::string audio, std::exception_ptr exception);

    explicit SpeechSynthesizeTask(ITextToSpeechClient& client,
                                  std::move_only_function<OnDone> callback);

    virtual ~SpeechSynthesizeTask() = default;

    [[nodiscard]] bool
    ready() const;

    void
    submit();

    virtual void
    perform()
        = 0;

protected:
    ITextToSpeechClient&
    client();

    void
    setResult(std::string audio);

    void
    setResult(std::exception_ptr exception);

    void
    reset();

private:
    ITextToSpeechClient& _client;
    std::move_only_function<OnDone> _callback;
    std::string _audio;
    std::exception_ptr _exception;
    bool _ready;
};

class TextSpeechSynthesizeTask final : public SpeechSynthesizeTask {
public:
    TextSpeechSynthesizeTask(ITextToSpeechClient& client,
                             std::string_view text,
                             std::string_view lang,
                             std::move_only_function<OnDone> callback);

    void
    perform() override;

private:
    std::string _text;
    std::string _lang;
};

class SsmlSpeechSynthesizeTask final : public SpeechSynthesizeTask {
public:
    SsmlSpeechSynthesizeTask(ITextToSpeechClient& client,
                             std::string_view ssml,
                             std::string_view lang,
                             std::move_only_function<OnDone> callback);

    void
    perform() override;

private:
    std::string _ssml;
    std::string _lang;
};

} // namespace jar
