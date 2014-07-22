#ifndef PLAYER_H
#define PLAYER_H

#include "creature.h"

#define _Player Player::inst()

class Location;

class Player : public Creature
{
public:
  static uint SightRange;

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

  Location *_current_loc;
  Location *_prev_loc;

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

  Location* get_location() const { return _current_loc; }
  Location* get_prev_loc() const { return _prev_loc; }
  void set_location(Location* loc);
};

#endif // PLAYER_H
