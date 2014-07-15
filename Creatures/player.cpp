#include "player.h"

Player* Player::_player = nullptr;

Player::Player() : Creature(5)
{
}

Player &Player::inst()
{
  if (_player == nullptr)
  {
    _player = new Player;
    _player->load();
  }

  return *_player;
}
