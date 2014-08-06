#ifndef NPC_H
#define NPC_H

#include "creature.h"

#include "Include/enums/e_clan.h"
#include "Include/enums/e_tribe.h"

class NPC : public Creature
{
public:
  virtual ~NPC();

  Clan clan() const { return _clan; }
  Tribe tribe() const { return _tribe; }

  void set_clan(Clan clan);
  void set_tribe(Tribe tribe);

  virtual void saveToDB();
  virtual void load(MapRow *data_source);

private:
  friend class Creature;
  NPC(dbRef ref, bool temporary = false);

  Clan _clan;
  Tribe _tribe;

};

#endif // NPC_H
