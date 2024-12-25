#pragma once

#include "speechee/IPlayerFactory.hpp"

#include <gmock/gmock.h>

namespace jar {

class MockPlayerFactory : public IPlayerFactory {
public:
    MockPlayerFactory();

    MOCK_METHOD(std::shared_ptr<IPlayer>, create, (), (override));
};

} // namespace jar
