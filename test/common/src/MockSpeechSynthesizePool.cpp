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
