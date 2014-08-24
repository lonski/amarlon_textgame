#include "testcreaturemodificatormanager.h"
#include "../Creatures/creaturemodificatormanager.h"

using namespace std;

TestCreatureModificatorManager::TestCreatureModificatorManager()
{
}

void TestCreatureModificatorManager::modmanager()
{
  //create manager
  CreatureModificatorManager manager;
  //create some mods
  shared_ptr<CreatureModificator> mod1(new CreatureModificator), mod2(new CreatureModificator), mod3(new CreatureModificator);

  //set the mods
  mod1->creature_stats().setAttribute(Attribute::CHR, 1);
  mod1->creature_stats().setAttribute(Attribute::STR, 1);
  mod1->saveToDB();

  mod2->creature_stats().setAttribute(Attribute::CHR, 2);
  mod2->creature_stats().setAttribute(Attribute::DEX, 1);
  mod2->saveToDB();

  mod3->creature_stats().setAttribute(Attribute::CHR, 3);
  mod3->creature_stats().setAttribute(Attribute::IMP, 1);
  mod3->saveToDB();

  //ADD mods to manager
  manager.add(mod1);
  manager.add(mod2);
  manager.add(mod3);

  //validate size
  QCOMPARE(manager.getAll().size(), (size_t)3);
  //validate complex mod
  QCOMPARE(manager.get_complex_mod()->creature_stats().attribute(Attribute::CHR), 6);
  QCOMPARE(manager.get_complex_mod()->creature_stats().attribute(Attribute::STR), 1);
  QCOMPARE(manager.get_complex_mod()->creature_stats().attribute(Attribute::DEX), 1);
  QCOMPARE(manager.get_complex_mod()->creature_stats().attribute(Attribute::IMP), 1);

  //REMOVE mod2
  manager.remove(mod2->ref());

  //validate size
  QCOMPARE(manager.getAll().size(), (size_t)2);
  //validate complex mod
  QCOMPARE(manager.get_complex_mod()->creature_stats().attribute(Attribute::CHR), 4);
  QCOMPARE(manager.get_complex_mod()->creature_stats().attribute(Attribute::STR), 1);
  QCOMPARE(manager.get_complex_mod()->creature_stats().attribute(Attribute::DEX), 0);
  QCOMPARE(manager.get_complex_mod()->creature_stats().attribute(Attribute::IMP), 1);

  mod1->purge();
  mod2->purge();
  mod3->purge();
}

void TestCreatureModificatorManager::modmanager_ticktime()
{
  //create manager
  CreatureModificatorManager manager;
  //create some mods
  shared_ptr<CreatureModificator> mod1(new CreatureModificator), mod2(new CreatureModificator),
                                  mod3(new CreatureModificator), mod4(new CreatureModificator);
  //set mods time
  mod1->set_effect_time(10);
  mod1->saveToDB();
  mod2->set_effect_time(10);
  mod2->saveToDB();
  mod3->set_effect_time(20);
  mod3->saveToDB();
  mod4->saveToDB();

  //ADD mods to manager
  manager.add(mod1);
  manager.add(mod2);
  manager.add(mod3);
  manager.add(mod4);

  //validate size
  QCOMPARE(manager.getAll().size(), (size_t)4);

  //TICK 4
  GameClock::Clock().tick_time(4);

  //validate size
  QCOMPARE(manager.getAll().size(), (size_t)4);

  //TICK 10
  GameClock::Clock().tick_time(10);

  //validate size
  QCOMPARE(manager.getAll().size(), (size_t)2);

  //TICK 6
  GameClock::Clock().tick_time(6);

  //validate size
  QCOMPARE(manager.getAll().size(), (size_t)1);

  mod1->purge();
  mod2->purge();
  mod3->purge();
  mod4->purge();
}
