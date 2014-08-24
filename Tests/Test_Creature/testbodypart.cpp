#include "testbodypart.h"
#include "../Creatures/bodypart.h"

using namespace std;

TestBodyPart::TestBodyPart()
{
}

void TestBodyPart::bodypart_tostr()
{
  BodyPart bp;
  bp.setRegion(BodyRegion::Dol);
  bp.setSide(BodySide::Left);
  bp.setType(BodyPartType::Noga);
  bp.setDamage(DamageLevel::Brak);

  QCOMPARE( bp.toStr().c_str(), "2,1,4,1,0,0,0,0,0,0,0,0,");
}

void TestBodyPart::bodypart_fromstr()
{
  Item::STLContainer eq;
  BodyPart bp("2,1,4,1,0,0,0,0,0,0,0,0,", eq);

  QVERIFY(bp.region() == BodyRegion::Dol);
  QVERIFY(bp.side() == BodySide::Left);
  QVERIFY(bp.type() == BodyPartType::Noga);
  QVERIFY(bp.damage() == DamageLevel::Brak);
  QVERIFY(eq.empty());
}

void TestBodyPart::bodypart_equip()
{
  Item::STLContainer eq;
  BodyPart bp("2,1,4,1,0,0,0,0,0,0,0,0,", eq);
  ItemPtr item( Item::prototypes().clone(ItemPrototype::Noz) );

  bp.equip(item);
  QVERIFY(bp.equipped().size() == 1);

  string str = "2,1,4,1,0,0,"+fun::toStr(item->ref())+",0,0,0,0,0,";

  QCOMPARE(bp.toStr().c_str(), str.c_str());
  //qDebug() << bp.toStr().c_str();

  item->purge();
}

void TestBodyPart::bodypart_unequip()
{
  //equip
  Item::STLContainer eq;
  BodyPart bp("2,1,4,1,0,0,0,0,0,0,0,0,", eq);
  ItemPtr item( Item::prototypes().clone(ItemPrototype::Noz) );

  bp.equip(item);
  QVERIFY(!bp.equipped().empty());
  //~~~

  ItemPtr i2 = bp.unequip().at(0);
  QVERIFY(bp.equipped().empty());
  QCOMPARE(i2.get(), item.get());

  item->purge();
}

void TestBodyPart::bodypart_creation_fromstr()
{
  Item::STLContainer eq;
  BodyPart bp("2,1,4,1,121,0,0,0,0,0,0,0,", eq);

  QVERIFY(bp.region() == BodyRegion::Dol);
  QVERIFY(bp.side() == BodySide::Left);
  QVERIFY(bp.type() == BodyPartType::Noga);
  QVERIFY(bp.damage() == DamageLevel::Brak);
  QVERIFY(!bp.equipped().empty());
  QCOMPARE(bp.equipped().at(0).lock()->ref(), (dbRef)121);
  QCOMPARE(bp.toStr().c_str(), "2,1,4,1,121,0,0,0,0,0,0,0,");
}
