#include "testcreaturemodificator.h"
#include "../Creatures/creaturemodificator.h"

TestCreatureModificator::TestCreatureModificator()
{
}

void TestCreatureModificator::modificator_creation()
{
  CreatureModificator mod;
  mod.saveToDB();
  QVERIFY(mod.ref() != 0);
  mod.purge();
}

void TestCreatureModificator::modificator_creation_existing()
{
  CreatureModificator mod(13);
  QCOMPARE(mod.ref(), (dbRef)13);
}

void TestCreatureModificator::modificator_load()
{
  CreatureModificator mod(13);

  QCOMPARE(mod.name().c_str(), "testo");
  QCOMPARE(mod.global_test_level_mod(), 1);
  QCOMPARE(mod.effect_time(), 9);
  QCOMPARE(mod.creature_stats().attribute(Attribute::CHR), 3);
  QCOMPARE(mod.creature_stats().attribute(Attribute::STR), 0);
  QCOMPARE(mod.creature_stats().skill(Skill::Czujnosc), 34);
  QCOMPARE(mod.creature_stats().skill(Skill::Akrobatyka), 0);
}

void TestCreatureModificator::modificator_save()
{
  //create blank one
  CreatureModificator* mod = new CreatureModificator;

  //create record in db
  mod->saveToDB();

  //set some data
  mod->creature_stats().setAttribute(Attribute::DEX, 3);
  mod->setName("lol");

  //store ref
  dbRef ref = mod->ref();

  delete mod;

  //recreate
  mod = new CreatureModificator(ref);

  //validate
  QCOMPARE(mod->name().c_str(), "lol");
  QVERIFY(mod->creature_stats().attribute(Attribute::DEX) == 3);

  //clean up!
  mod->purge();
}

void TestCreatureModificator::modificator_augument()
{
  CreatureModificator m1, m2;

  //set some stats
  m1.creature_stats().setAttribute(Attribute::IMP, 3);
  m1.creature_stats().setSkill(Skill::Gornictwo, 34);
  m1.set_global_test_level_mod(1);

  QCOMPARE(m1.creature_stats().attribute(Attribute::IMP), 3);
  QCOMPARE(m1.creature_stats().skill(Skill::Gornictwo), 34);
  QCOMPARE(m1.global_test_level_mod(), 1);
  QCOMPARE(m1.effect_time(), -1);

  //set augument stats
  m2.creature_stats().setAttribute(Attribute::IMP, 1);
  m2.creature_stats().setAttribute(Attribute::STR, 3);
  m2.creature_stats().setSkill(Skill::Odpornosc, 4);
  m2.set_global_test_level_mod(2);
  m2.set_effect_time(50);

  QCOMPARE(m2.creature_stats().attribute(Attribute::IMP), 1);
  QCOMPARE(m2.creature_stats().attribute(Attribute::STR), 3);
  QCOMPARE(m2.creature_stats().skill(Skill::Odpornosc), 4);
  QCOMPARE(m2.global_test_level_mod(), 2);
  QCOMPARE(m2.effect_time(), 50);

  //augument
  m1.augument(m2);

  //validate
  QCOMPARE(m1.creature_stats().attribute(Attribute::IMP), 4);
  QCOMPARE(m1.creature_stats().attribute(Attribute::STR), 3);
  QCOMPARE(m1.creature_stats().skill(Skill::Gornictwo), 34);
  QCOMPARE(m1.creature_stats().skill(Skill::Odpornosc), 4);
  QCOMPARE(m1.global_test_level_mod(), 3);
  QCOMPARE(m1.effect_time(), -1);

  //remove augument
  m1.remove_augument(m2);

  QCOMPARE(m1.creature_stats().attribute(Attribute::IMP), 3);
  QCOMPARE(m1.creature_stats().attribute(Attribute::STR), 0);
  QCOMPARE(m1.creature_stats().skill(Skill::Gornictwo), 34);
  QCOMPARE(m1.creature_stats().skill(Skill::Odpornosc), 0);
  QCOMPARE(m1.global_test_level_mod(), 1);
  QCOMPARE(m1.effect_time(), -1);
}
