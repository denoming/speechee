#include "speechee/SpeechSynthesizeTask.hpp"

#include "tts/ITextToSpeechClient.hpp"

#include <gsl/gsl-lite.hpp>

#include <stdexcept>

namespace jar {

SpeechSynthesizeTask::SpeechSynthesizeTask(ITextToSpeechClient& client,
                                           std::move_only_function<OnDone> callback)
    : _client{client}
    , _callback{std::move(callback)}
    , _ready{false}
{
    gsl_Expects(_callback);
}

bool
SpeechSynthesizeTask::ready() const
{
    return _ready;
}

void
SpeechSynthesizeTask::submit()
{
    if (ready()) {
        _callback(std::move(_audio), std::move(_exception));
        reset();
    } else {
        throw std::logic_error{"Attempt to submit not ready task"};
    }
}

ITextToSpeechClient&
SpeechSynthesizeTask::client()
{
    return _client;
}

void
SpeechSynthesizeTask::setResult(std::string audio)
{
    _audio = std::move(audio);
    _ready = true;
}

void
SpeechSynthesizeTask::setResult(std::exception_ptr exception)
{
    _exception = std::move(exception);
    _ready = true;
}

void
SpeechSynthesizeTask::reset()
{
    _ready = false;
    _exception = nullptr;
}

} // namespace jar

namespace jar {

TextSpeechSynthesizeTask::TextSpeechSynthesizeTask(ITextToSpeechClient& client,
                                                   std::string_view text,
                                                   std::string_view lang,
                                                   std::move_only_function<OnDone> callback)
    : SpeechSynthesizeTask{client, std::move(callback)}
    , _text{text}
    , _lang{lang}
{
}

void
TextSpeechSynthesizeTask::perform()
{
    try {
        setResult(client().synthesizeText(_text, {.languageCode = _lang}, {}));
    } catch (...) {
        setResult(std::current_exception());
    }
}

} // namespace jar

namespace jar {

SsmlSpeechSynthesizeTask::SsmlSpeechSynthesizeTask(ITextToSpeechClient& client,
                                                   std::string_view ssml,
                                                   std::string_view lang,
                                                   std::move_only_function<OnDone> callback)
    : SpeechSynthesizeTask{client, std::move(callback)}
    , _ssml{ssml}
    , _lang{lang}
{
}

void
SsmlSpeechSynthesizeTask::perform()
{
    try {
        setResult(client().synthesizeSsml(_ssml, {.languageCode = _lang}, {}));
    } catch (...) {
        setResult(std::current_exception());
    }
}

} // namespace jar
