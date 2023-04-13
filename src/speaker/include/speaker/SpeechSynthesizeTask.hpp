#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <system_error>

namespace jar {

class ITextToSpeechClient;

class SpeechSynthesizeTask {
public:
    using OnDone = void(std::string audio, std::error_code error);

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
    setResult(std::error_code error);

    void
    reset();

private:
    ITextToSpeechClient& _client;
    std::move_only_function<OnDone> _callback;
    std::string _audio;
    std::error_code _error;
    bool _ready;
};

class TextSpeechSynthesizeTask final : public SpeechSynthesizeTask {
public:
    TextSpeechSynthesizeTask(ITextToSpeechClient& client,
                             std::string_view text,
                             std::string_view lang,
                             std::move_only_function<OnDone> callback);

    void
    perform() final;

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
    perform() final;

private:
    std::string _ssml;
    std::string _lang;
};

} // namespace jar
