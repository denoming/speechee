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
