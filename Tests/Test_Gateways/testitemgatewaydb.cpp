#include "testitemgatewaydb.h"
#include "Equipment/item.h"

TestItemGatewayDB::TestItemGatewayDB()
{
  item = dynamic_cast<Item*>(itemGateway.fetch(1));
  item_to_clone = dynamic_cast<Item*>(itemGateway.fetch(122));
  cloned = dynamic_cast<Item*>(itemGateway.clone(item_to_clone));

  mod_cloned = cloned->mods()->getAll().at(0);
  mod_source = item_to_clone->mods()->getAll().at(0);

}

TestItemGatewayDB::~TestItemGatewayDB()
{
  cloned->purge();
  delete cloned;
  delete item;
}

void TestItemGatewayDB::fetchNonExistingOjects_returnsNull()
{
  QVERIFY(itemGateway.fetch(0) == nullptr);
  QVERIFY(itemGateway.fetch(666) == nullptr);
}

void TestItemGatewayDB::fetchExistingObject_returnsItem()
{
  QVERIFY(item != nullptr);
}

void TestItemGatewayDB::fetchedItemHasValidData()
{
  QCOMPARE(item->ref(), (dbRef)1);
  QCOMPARE(item->value(), 43);
  QCOMPARE(item->attack(), 2);
  QCOMPARE(item->damageReduction().bashing, 0);
}

void TestItemGatewayDB::updateingExistingItemInDataSource()
{
  QCOMPARE(item->ref(), (dbRef)1);
  QCOMPARE(item->value(), 43);

  //change sth and save
  item->setValue(8);
  itemGateway.write(item);

  //reload and check
  item = dynamic_cast<Item*>(itemGateway.fetch(1));
  QCOMPARE(item->value(), 8);

  //restore
  item->setValue(43);
  itemGateway.write(item);

}

void TestItemGatewayDB::insertNewItemIntoDataSource()
{
  QCOMPARE(item->ref(), (dbRef)1);

  //change ref to some non-existing
  item->setRef(777);
  //write to ds and aquire new id
  unsigned int new_ref = itemGateway.write(item);
  QVERIFY(new_ref != (unsigned int)777);

  //reload and check
  item = dynamic_cast<Item*>(itemGateway.fetch(new_ref));
  QCOMPARE(item->ref(), new_ref);

  //clean up
  item->purge();
  item = dynamic_cast<Item*>(itemGateway.fetch(1));
}

void TestItemGatewayDB::clonedItemIsValidPointer()
{
  QVERIFY(cloned != nullptr);
}

void TestItemGatewayDB::clonedItemHasValidId()
{
  QVERIFY(cloned->ref() > 0);
  QVERIFY(cloned->ref() != item->ref());
}

void TestItemGatewayDB::clonedItemHasSameData()
{
  QCOMPARE(cloned->table().c_str(), item_to_clone->table().c_str());
  QCOMPARE(cloned->name().c_str(), item_to_clone->name().c_str());
  QCOMPARE(cloned->type(), item_to_clone->type());
  QCOMPARE(cloned->descript().c_str(), item_to_clone->descript().c_str());
  QCOMPARE(cloned->weight(), item_to_clone->weight());
  QCOMPARE(cloned->value(), item_to_clone->value());
  QCOMPARE(cloned->condition(), item_to_clone->condition());
  QCOMPARE(cloned->durability(), item_to_clone->durability());
  QCOMPARE(cloned->isStackable(), item_to_clone->isStackable());
  QCOMPARE(cloned->weaponSkill(), item_to_clone->weaponSkill());
  QCOMPARE(cloned->damage(), item_to_clone->damage());
  QCOMPARE(cloned->defence(), item_to_clone->defence());
  QCOMPARE(cloned->attack(), item_to_clone->attack());
  QCOMPARE(cloned->reflex(), item_to_clone->reflex());
  QCOMPARE(cloned->str_req(), item_to_clone->str_req());
  QCOMPARE(cloned->range(), item_to_clone->range());
  QCOMPARE(cloned->damageReduction(), item_to_clone->damageReduction());
  QCOMPARE(cloned->hunger(), item_to_clone->hunger());
}

void TestItemGatewayDB::clonedItemHasSameBodyParts()
{
  std::vector<BodyPartType> cloned_parts = cloned->bodyParts();
  std::vector<BodyPartType> item_parts = item_to_clone->bodyParts();

  QCOMPARE(cloned_parts.size(), item_parts.size());

  for (auto cp = cloned_parts.begin(); cp != cloned_parts.end(); ++cp)
    QVERIFY(item_to_clone->checkBodyPart(*cp));

}

void TestItemGatewayDB::clonedItemHasSameModificatorsCount()
{
  QCOMPARE(cloned->mods()->count(), item_to_clone->mods()->count());
}

void TestItemGatewayDB::clonedItemModificatorHasValidRef()
{
  QVERIFY(mod_cloned->ref() > 0);
  QVERIFY(mod_cloned->ref() != mod_source->ref());
}

void TestItemGatewayDB::clonedItemModificatorHasValidOwner()
{
  QCOMPARE(cloned->mods()->owner(), cloned);
  QCOMPARE(mod_cloned->oref(), cloned->ref());
  QCOMPARE(mod_cloned->otable(), cloned->table());
}

void TestItemGatewayDB::clonedItemModificatorHasSameData()
{
  QVERIFY(mod_source->attribute(Attribute::DEX) != 0);
  QCOMPARE(mod_cloned->attribute(Attribute::DEX), mod_source->attribute(Attribute::DEX));
}
