#ifndef MOB_H
#define MOB_H

#include "creature.h"

class MOB : public Creature
{
public:
  virtual ~MOB() {}

private:
  friend class Creature;
  MOB(dbRef ref);

};

#endif // MOB_H
