#ifndef MOB_H
#define MOB_H

#include "creature.h"

class MOB : public Creature
{
private:
  friend class Creature;
  MOB(dbRef ref, bool temporary = false);
public:
  virtual ~MOB() {}
};

#endif // MOB_H
