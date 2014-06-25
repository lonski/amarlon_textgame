#include "testcreature.h"

using namespace std;

void TestCreature::stats_atr2str()
{
  CreatureStats stats;

  stats.set_attribute(Attribute::CHR, 2);
  stats.set_attribute(Attribute::IMP, 5);
  stats.set_attribute(Attribute::STR, 7);
  stats.set_attribute(Attribute::DEX, 3);

  QCOMPARE(stats.Attributes2Str().c_str(), "0;7;3;0;0;0;2;5;0;");
}

void TestCreature::stats_str2atrs()
{
  CreatureStats stats;

  stats.Str2Attributes("0;7;3;9;0;0;2;5;0;");

  QCOMPARE(stats.get_attribute(Attribute::CHR), 2);
  QCOMPARE(stats.get_attribute(Attribute::IMP), 5);
  QCOMPARE(stats.get_attribute(Attribute::STR), 7);
  QCOMPARE(stats.get_attribute(Attribute::DEX), 3);
  QCOMPARE(stats.get_attribute(Attribute::END), 9);
  QCOMPARE(stats.get_attribute(Attribute::REF), 0);
  QCOMPARE(stats.get_attribute(Attribute::INT), 0);
  QCOMPARE(stats.get_attribute(Attribute::WLL), 0);
}

void TestCreature::stats_skills2str()
{
  CreatureStats stats;

  stats.set_skill(Skill::Aktorstwo, 43);
  stats.set_skill(Skill::Empatia, 12);
  stats.set_skill(Skill::Mocna_glowa, 67);

  QCOMPARE( stats.Skills2Str().c_str(), "0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;12;0;0;0;0;0;0;0;0;43;0;0;0;0;0;0;0;0;0;0;0;67;0;0;0;0;0;0;");
}

void TestCreature::stats_str2skills()
{
  CreatureStats stats;

  stats.Str2Skills("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;12;0;0;0;0;0;0;0;0;43;0;0;0;0;0;0;0;0;0;0;0;67;0;0;0;0;0;1;");

  QCOMPARE(stats.get_skill(Skill::Aktorstwo), 43);
  QCOMPARE(stats.get_skill(Skill::Empatia), 12);
  QCOMPARE(stats.get_skill(Skill::Mocna_glowa), 67);
  QCOMPARE(stats.get_skill(Skill::Taniec), 1);
  QCOMPARE(stats.get_skill(Skill::Taktyka), 0);
}

void TestCreature::bodypart_tostr()
{
  BodyPart bp;
  bp.set_region(BodyRegion::Dol);
  bp.set_type(BodyPartType::LeftLeg);
  bp.set_damage(DamageLevel::Brak);

  QCOMPARE( bp.toStr().c_str(), "2,5,1,0");
}

void TestCreature::bodypart_fromstr()
{
  BodyPart bp("2,5,1,0");

  QVERIFY(bp.region() == BodyRegion::Dol);
  QVERIFY(bp.type() == BodyPartType::LeftLeg);
  QVERIFY(bp.damage() == DamageLevel::Brak);
  QVERIFY(bp.equipped().lock() == nullptr);
}

void TestCreature::bodypart_equip()
{
  BodyPart bp("2,5,1,0");
  shared_ptr<Item> item( Item::prototypes().clone(ItemPrototype::Noz).release() );

  bp.equip(item);
  QVERIFY(bp.equipped().lock() != nullptr);

  string str = "2,5,1,"+fun::toStr(item->ref());
  QCOMPARE(bp.toStr().c_str(), str.c_str());

  item->purge();
}

void TestCreature::bodypart_unequip()
{
  //equip
  BodyPart bp("2,5,1,0");
  shared_ptr<Item> item( Item::prototypes().clone(ItemPrototype::Noz).release() );

  bp.equip(item);
  QVERIFY(bp.equipped().lock() != nullptr);

  string str = "2,5,1,"+fun::toStr(item->ref());
  QCOMPARE(bp.toStr().c_str(), str.c_str());
  //~~~

  shared_ptr<Item> i2 = bp.unequip();
  QVERIFY(bp.equipped().lock() == nullptr);
  QCOMPARE(i2.get(), item.get());

  item->purge();
}

void TestCreature::bodypart_creation_fromstr()
{
  BodyPart bp("2,5,1,121");

  QVERIFY(bp.region() == BodyRegion::Dol);
  QVERIFY(bp.type() == BodyPartType::LeftLeg);
  QVERIFY(bp.damage() == DamageLevel::Brak);
  QVERIFY(bp.equipped().lock() != nullptr);
  QCOMPARE(bp.equipped().lock()->ref(), (dbRef)121);
  QCOMPARE(bp.toStr().c_str(), "2,5,1,121");
}

void TestCreature::modificator_creation()
{
  CreatureModificator mod;
  mod.save_to_db();
  QVERIFY(mod.ref() != 0);
  mod.purge();
}

void TestCreature::modificator_creation_existing()
{
  CreatureModificator mod(13);
  QCOMPARE(mod.ref(), (dbRef)13);
}

void TestCreature::modificator_load()
{
  CreatureModificator mod(13);

  QCOMPARE(mod.name().c_str(), "testo");
  QCOMPARE(mod.global_test_level_mod(), 1);
  QCOMPARE(mod.effect_time(), 9);
  QCOMPARE(mod.creature_stats().get_attribute(Attribute::CHR), 3);
  QCOMPARE(mod.creature_stats().get_attribute(Attribute::STR), 0);
  QCOMPARE(mod.creature_stats().get_skill(Skill::Czujnosc), 34);
  QCOMPARE(mod.creature_stats().get_skill(Skill::Akrobatyka), 0);
}

void TestCreature::modificator_save()
{
  //create blank one
  CreatureModificator* mod = new CreatureModificator;

  //create record in db
  mod->save_to_db();

  //set some data
  mod->creature_stats().set_attribute(Attribute::DEX, 3);
  mod->set_name("lol");

  //store ref
  dbRef ref = mod->ref();

  delete mod;

  //recreate
  mod = new CreatureModificator(ref);

  //validate
  QCOMPARE(mod->name().c_str(), "lol");
  QVERIFY(mod->creature_stats().get_attribute(Attribute::DEX) == 3);

  //clean up!
  mod->purge();
}

void TestCreature::modificator_augument()
{
  CreatureModificator m1, m2;

  //set some stats
  m1.creature_stats().set_attribute(Attribute::IMP, 3);
  m1.creature_stats().set_skill(Skill::Gornictwo, 34);
  m1.set_global_test_level_mod(1);

  QCOMPARE(m1.creature_stats().get_attribute(Attribute::IMP), 3);
  QCOMPARE(m1.creature_stats().get_skill(Skill::Gornictwo), 34);
  QCOMPARE(m1.global_test_level_mod(), 1);
  QCOMPARE(m1.effect_time(), -1);

  //set augument stats
  m2.creature_stats().set_attribute(Attribute::IMP, 1);
  m2.creature_stats().set_attribute(Attribute::STR, 3);
  m2.creature_stats().set_skill(Skill::Odpornosc, 4);
  m2.set_global_test_level_mod(2);
  m2.set_effect_time(50);

  QCOMPARE(m2.creature_stats().get_attribute(Attribute::IMP), 1);
  QCOMPARE(m2.creature_stats().get_attribute(Attribute::STR), 3);
  QCOMPARE(m2.creature_stats().get_skill(Skill::Odpornosc), 4);
  QCOMPARE(m2.global_test_level_mod(), 2);
  QCOMPARE(m2.effect_time(), 50);

  //augument
  m1.augument(m2);

  //validate
  QCOMPARE(m1.creature_stats().get_attribute(Attribute::IMP), 4);
  QCOMPARE(m1.creature_stats().get_attribute(Attribute::STR), 3);
  QCOMPARE(m1.creature_stats().get_skill(Skill::Gornictwo), 34);
  QCOMPARE(m1.creature_stats().get_skill(Skill::Odpornosc), 4);
  QCOMPARE(m1.global_test_level_mod(), 3);
  QCOMPARE(m1.effect_time(), -1);

  //remove augument
  m1.remove_augument(m2);

  QCOMPARE(m1.creature_stats().get_attribute(Attribute::IMP), 3);
  QCOMPARE(m1.creature_stats().get_attribute(Attribute::STR), 0);
  QCOMPARE(m1.creature_stats().get_skill(Skill::Gornictwo), 34);
  QCOMPARE(m1.creature_stats().get_skill(Skill::Odpornosc), 0);
  QCOMPARE(m1.global_test_level_mod(), 1);
  QCOMPARE(m1.effect_time(), -1);
}

