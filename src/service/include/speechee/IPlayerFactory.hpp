#pragma once

#include "speechee/IPlayer.hpp"

#include <memory>

namespace jar {

class IPlayerFactory {
public:
    virtual ~IPlayerFactory() = default;

    [[nodiscard]] virtual std::shared_ptr<IPlayer>
    create() = 0;
};

} // namespace jar