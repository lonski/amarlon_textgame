#ifndef NPC_H
#define NPC_H

#include "creature.h"

class NPC : public Creature
{
private:
  friend class Creature;
  NPC(dbRef ref, bool temporary = false);
public:
  virtual ~NPC() {}
};

#endif // NPC_H
