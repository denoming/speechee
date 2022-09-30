#include "speech/SpeechSynthesizeTask.hpp"

#include "speech/ITextToSpeechClient.hpp"

#include <boost/assert.hpp>

#include <stdexcept>

namespace jar {

SpeechSynthesizeTask::SpeechSynthesizeTask(ITextToSpeechClient& client, Callback callback)
    : _client{client}
    , _callback{std::move(callback)}
    , _ready{false}
{
    BOOST_ASSERT_MSG(_callback, "Invalid callback");
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
        _callback(std::move(_audioContent), _error);
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
SpeechSynthesizeTask::setResult(std::string audioContent)
{
    _audioContent = std::move(audioContent);
    _ready = true;
}

void
SpeechSynthesizeTask::setResult(std::error_code error)
{
    _error = error;
    _ready = true;
}

void
SpeechSynthesizeTask::reset()
{
    _ready = false;
    _error = {};
}

} // namespace jar

namespace jar {

TextSpeechSynthesizeTask::TextSpeechSynthesizeTask(ITextToSpeechClient& client,
                                                   std::string_view text,
                                                   std::string_view lang,
                                                   Callback callback)
    : SpeechSynthesizeTask{client, std::move(callback)}
    , _text{text}
    , _lang{lang}
{
}

void
TextSpeechSynthesizeTask::perform()
{
    std::error_code error;
    auto content = client().synthesizeText(_text, _lang, error);
    if (error) {
        setResult(error);
    } else {
        setResult(std::move(content));
    }
}

} // namespace jar

namespace jar {

SsmlSpeechSynthesizeTask::SsmlSpeechSynthesizeTask(ITextToSpeechClient& client,
                                                   std::string_view ssml,
                                                   std::string_view lang,
                                                   Callback callback)
    : SpeechSynthesizeTask{client, std::move(callback)}
    , _ssml{ssml}
    , _lang{lang}
{
}

void
SsmlSpeechSynthesizeTask::perform()
{
    std::error_code error;
    auto content = client().synthesizeSsml(_ssml, _lang, error);
    if (error) {
        setResult(error);
    } else {
        setResult(std::move(content));
    }
}

} // namespace jar