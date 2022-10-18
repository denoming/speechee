#include "test/MockSpeechSynthesizePool.hpp"

using namespace testing;

namespace jar {

MockSpeechSynthesizePool::MockSpeechSynthesizePool()
{
    ON_CALL(*this, synthesizeText)
        .WillByDefault(
            InvokeArgument<2>(std::string{}, std::make_error_code(std::errc::invalid_argument)));

    ON_CALL(*this, synthesizeSsml)
        .WillByDefault(
            InvokeArgument<2>(std::string{}, std::make_error_code(std::errc::invalid_argument)));
}

} // namespace jar
