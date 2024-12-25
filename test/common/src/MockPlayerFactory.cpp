#include "speechee/MockPlayerFactory.hpp"

#include "speechee/MockPlayer.hpp"

using namespace testing;

namespace jar {

MockPlayerFactory::MockPlayerFactory()
{
    ON_CALL(*this, create).WillByDefault(Return(std::make_shared<MockPlayer>()));
}

} // namespace jar
