#include "creaturemodificator.h"

void CreatureModificator::augument(const CreatureModificator &mod)
{
  _mods.augument(creature_stats());
  _global_test_level_mod += mod.global_test_level_mod();
}

void CreatureModificator::remove_augument(const CreatureModificator &mod)
{
  _mods.remove_augument(creature_stats());
  _global_test_level_mod -= mod.global_test_level_mod();
}


CreatureModificator::CreatureModificator()
{
  //TODO: create new record in db
}

CreatureModificator::CreatureModificator(dbRef ref)
{
  //TODO: load existing record from db
}
