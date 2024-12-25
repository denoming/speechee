#pragma once

#include "speaker/IPlayerFactory.hpp"
#include "speaker/IPlayerLoop.hpp"

namespace jar {

class PlayerFactory final : public IPlayerFactory {
public:
    explicit PlayerFactory(IPlayerLoop& playerLoop);

    [[nodiscard]] std::shared_ptr<IPlayer>
    create() override;

private:
    IPlayerLoop& _playerLoop;
};

} // namespace jar
