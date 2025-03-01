#include "speechee/SpeechSynthesizePool.hpp"

#include <gsl/gsl-lite.hpp>

namespace jar {

SpeechSynthesizePool::SpeechSynthesizePool(ITextToSpeechClient& client, std::size_t threads)
    : _client{client}
{
    setUp(threads);
}

SpeechSynthesizePool::~SpeechSynthesizePool()
{
    tearDown();
}

void
SpeechSynthesizePool::synthesizeText(std::string_view text,
                                     std::string_view lang,
                                     std::move_only_function<OnDone> callback)
{
    gsl_Expects(not text.empty());
    gsl_Expects(not lang.empty());
    gsl_Expects(callback);

    pushTask(std::make_unique<TextSpeechSynthesizeTask>(_client, text, lang, std::move(callback)));
}

void
SpeechSynthesizePool::synthesizeSsml(std::string_view ssml,
                                     std::string_view lang,
                                     std::move_only_function<OnDone> callback)
{
    gsl_Expects(not ssml.empty());
    gsl_Expects(not lang.empty());
    gsl_Expects(callback);

    pushTask(std::make_unique<SsmlSpeechSynthesizeTask>(_client, ssml, lang, std::move(callback)));
}

void
SpeechSynthesizePool::setUp(std::size_t threads)
{
    gsl_Expects(threads > 0);

    while (threads--) {
        _threads.emplace_back(&SpeechSynthesizePool::threadFn, this);
    }
}

void
SpeechSynthesizePool::tearDown()
{
    _threads.clear();
}

void
SpeechSynthesizePool::threadFn(std::stop_token stoken)
{
    while (waitTask(stoken)) {
        auto task = nextTask();
        gsl_Assert(task);
        task->perform();
        task->submit();
    };
}

void
SpeechSynthesizePool::pushTask(std::unique_ptr<SpeechSynthesizeTask> task)
{
    std::unique_lock lock{_dataGuard};
    _tasks.push_back(std::move(task));
    lock.unlock();
    _dataPresent.notify_one();
}

bool
SpeechSynthesizePool::waitTask(std::stop_token stoken)
{
    std::unique_lock<std::mutex> lock{_dataGuard};
    _dataPresent.wait(lock, stoken, [this]() { return !_tasks.empty(); });
    return !stoken.stop_requested();
}

std::unique_ptr<SpeechSynthesizeTask>
SpeechSynthesizePool::nextTask()
{
    std::lock_guard lock{_dataGuard};
    gsl_Assert(not _tasks.empty());
    auto task = std::move(_tasks.front());
    _tasks.pop_front();
    return task;
}

} // namespace jar
