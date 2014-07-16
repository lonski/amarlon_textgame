#ifndef PLAYER_H
#define PLAYER_H

#include "creature.h"

#define _Player Player::inst()

class Player : public Creature
{
private:
  static const dbRef player_ref;
  static Player *_instance;
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
  static Player *inst();
  ~Player() {}

  Profession profession() const { return _prof; }
  Bless bless() const { return _bless; }
  Splot splot() const { return _splot; }
  Season birth_season() const { return _birth_season; }
  Tribe tribe() const { return _tribe; }
  Clan clan() const { return _clan; }

  void load(MapRow *data_source = nullptr);
  void save_to_db();

};

#endif // PLAYER_H
