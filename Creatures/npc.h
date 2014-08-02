#ifndef NPC_H
#define NPC_H

#include "creature.h"

class NPC : public Creature
{
private:
  friend class Creature;

  Clan _clan;
  Tribe _tribe;

  NPC(dbRef ref, bool temporary = false);
public:
  Clan clan() const { return _clan; }
  Tribe tribe() const { return _tribe; }

  void set_clan(Clan clan);
  void set_tribe(Tribe tribe);

  virtual void saveToDB();
  virtual void load(MapRow *data_source);

  virtual ~NPC();
};

#endif // NPC_H
