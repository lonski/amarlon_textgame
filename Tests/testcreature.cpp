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

