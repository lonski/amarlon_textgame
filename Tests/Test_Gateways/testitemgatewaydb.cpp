#include "testitemgatewaydb.h"
#include "Equipment/item.h"

TestItemGatewayDB::TestItemGatewayDB()
{
}

void TestItemGatewayDB::fetchNonExistingOjects_returnsNull()
{
  QVERIFY(itemGateway.fetch(0) == nullptr);
  QVERIFY(itemGateway.fetch(666) == nullptr);
}

void TestItemGatewayDB::fetchExistingObject_returnsItem()
{
  Item* item = dynamic_cast<Item*>(itemGateway.fetch(1));

  QVERIFY(item != nullptr);
}

void TestItemGatewayDB::fetchedItemHasValidData()
{
  Item* item = dynamic_cast<Item*>(itemGateway.fetch(1));
  QCOMPARE(item->ref(), (dbRef)1);
  QCOMPARE(item->value(), 43);
  QCOMPARE(item->attack(), 2);
  QCOMPARE(item->damageReduction().bashing, 0);
}

void TestItemGatewayDB::updateingExistingItemInDataSource()
{
  Item* item = dynamic_cast<Item*>(itemGateway.fetch(1));

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
  Item* item = dynamic_cast<Item*>(itemGateway.fetch(1));

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
}
