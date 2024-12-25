#include "speaker/PlayerFactory.hpp"

#include "speaker/Player.hpp"

namespace jar {

PlayerFactory::PlayerFactory(IPlayerLoop& playerLoop)
    : _playerLoop{playerLoop}
{
}

std::shared_ptr<IPlayer>
PlayerFactory::create()
{
    return std::make_shared<Player>(_playerLoop);
}

} // namespace jar