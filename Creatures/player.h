#ifndef PLAYER_H
#define PLAYER_H

#include "creature.h"

#define _Player Player::inst()

class Player : public Creature
{
private:
  const dbRef player_ref = 5;
  static Player *_player;
  friend class Creature;
  Player();

  Profession _prof;
  Bless _bless;
  Splot _splot;
  Season _birth_season;
  Tribe _tribe;
  Clan _clan;

  HungerLevel _hunger;
  FatigueLevel _fatigue;

  CMValue<int> _lift_cap;
  VLValue<int> _exp;
  VLValue<int> _fame;
  BravePoints _brave;

public:
  static Player& inst();
  ~Player() {}
};

#endif // PLAYER_H
