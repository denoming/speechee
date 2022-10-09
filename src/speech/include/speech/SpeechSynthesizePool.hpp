#pragma once

#include "speech/ISpeechSynthesizePool.hpp"
#include "speech/SpeechSynthesizeTask.hpp"

#include <condition_variable>
#include <list>
#include <thread>
#include <vector>

namespace jar {

class ITextToSpeechClient;

class SpeechSynthesizePool final : public ISpeechSynthesizePool {
public:
    explicit SpeechSynthesizePool(ITextToSpeechClient& client, std::size_t threads);

    ~SpeechSynthesizePool() override;

    void
    synthesizeText(std::string_view text,
                   std::string_view lang,
                   std::function<OnCompleteSig> callback) final;

    void
    synthesizeSsml(std::string_view ssml,
                   std::string_view lang,
                   std::function<OnCompleteSig> callback) final;

private:
    using Tasks = std::list<SpeechSynthesizeTask::Ptr>;

    void
    setUp(std::size_t threads);

    void
    tearDown();

    void
    threadFn(std::stop_token stoken);

    void
    pushTask(std::unique_ptr<SpeechSynthesizeTask> task);

    bool
    waitTask(std::stop_token stoken);

    SpeechSynthesizeTask::Ptr
    nextTask();

private:
    ITextToSpeechClient& _client;
    Tasks _tasks;
    mutable std::mutex _dataGuard;
    std::condition_variable_any _dataPresent;
    std::vector<std::jthread> _threads;
};

} // namespace jar