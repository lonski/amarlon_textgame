#ifndef PLAYER_H
#define PLAYER_H

#include "creature.h"

#define _Player Player::inst()

class Player : public Creature
{
public:
  static uint SightRange;

private:
  static const dbRef playerRef;
  static Player *_instance;
  friend class Creature;
  Player();

  Profession _prof;
  Bless _bless;
  Splot _splot;
  Season _birthSeason;
  Tribe _tribe;
  Clan _clan;

  HungerLevel _hunger;
  FatigueLevel _fatigue;

  CMValue<int> _liftCap;
  VLValue<int> _exp;
  VLValue<int> _fame;
  BravePoints _brave;

public:
  static Player *inst();
  ~Player() {}

  Profession profession() const { return _prof; }
  Bless bless() const { return _bless; }
  Splot splot() const { return _splot; }
  Season birthSeason() const { return _birthSeason; }
  Tribe tribe() const { return _tribe; }
  Clan clan() const { return _clan; }

  void load(MapRow *data_source = nullptr);
  void saveToDB();
};

#endif // PLAYER_H
