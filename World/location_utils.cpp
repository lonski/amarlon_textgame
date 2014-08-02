#include "location_utils.h"
#include "Creatures/creature.h"
#include "World/location.h"

void moveCreature(Creature* crt, Location *loc)
{
  crt->setLocation(loc);

}
