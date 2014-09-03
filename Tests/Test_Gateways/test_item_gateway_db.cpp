#include "test_item_gateway_db.h"
#include "Equipment/item.h"
#include "Equipment/item_container.h"

TestItemGatewayDB::TestItemGatewayDB()
{
  item = dynamic_cast<Item*>(itemGateway.fetch(1));
  item_to_clone = dynamic_cast<Item*>(itemGateway.fetch(122));
  cloned = dynamic_cast<Item*>(itemGateway.clone(item_to_clone));

  item_with_inventory = Item::forge(ItemPrototype::LekkaZbrojaSkorzana);

  mod_cloned = cloned->mods()->getAll().at(0);
  mod_source = item_to_clone->mods()->getAll().at(0);

}

TestItemGatewayDB::~TestItemGatewayDB()
{
  cloned->purge();
  delete cloned;
  delete item;
  item_with_inventory->purge();
  delete item_with_inventory;
}

void TestItemGatewayDB::purgeItemWithInventory(Item* cloned_item_with_inv)
{
  purgeInventory(cloned_item_with_inv);
  cloned_item_with_inv->purge();
  delete cloned_item_with_inv;
}

void TestItemGatewayDB::purgeInventory(Item* item_with_inventory)
{
  std::vector<AmountedItem> inv = item_with_inventory->inventory()->getAll();
  for(auto i = inv.begin(); i != inv.end(); ++i)
  {
    AmountedItem ai = *i;
    ai.item->purge();
    item_with_inventory->inventory()->erase(ai.item->ref(), ai.amount);
  }
}

void TestItemGatewayDB::insertItemIntoItemWithInventory(Item* item_to_insert)
{
  ItemPtr inventored_item_1( item_to_insert );
  item_with_inventory->inventory()->insert(inventored_item_1);
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

void TestItemGatewayDB::fetchedItem_HasValidData()
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

void TestItemGatewayDB::clonedItem_IsValidPointer()
{
  QVERIFY(cloned != nullptr);
}

void TestItemGatewayDB::clonedItem_HasValidId()
{
  QVERIFY(cloned->ref() > 0);
  QVERIFY(cloned->ref() != item->ref());
}

void TestItemGatewayDB::clonedItem_HasSameData()
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

void TestItemGatewayDB::clonedItem_HasSameBodyParts()
{
  std::vector<BodyPartType> cloned_parts = cloned->bodyParts();
  std::vector<BodyPartType> item_parts = item_to_clone->bodyParts();

  QCOMPARE(cloned_parts.size(), item_parts.size());

  for (auto cp = cloned_parts.begin(); cp != cloned_parts.end(); ++cp)
    QVERIFY(item_to_clone->checkBodyPart(*cp));

}

void TestItemGatewayDB::clonedItem_HasSameModificatorsCount()
{
  QCOMPARE(cloned->mods()->count(), item_to_clone->mods()->count());
}

void TestItemGatewayDB::clonedItemModificator_HasValidRef()
{
  QVERIFY(mod_cloned->ref() > 0);
  QVERIFY(mod_cloned->ref() != mod_source->ref());
}

void TestItemGatewayDB::clonedItemModificator_HasValidOwner()
{
  QCOMPARE(cloned->mods()->owner(), cloned);
  QCOMPARE(mod_cloned->oref(), cloned->ref());
  QCOMPARE(mod_cloned->otable(), cloned->table());
}

void TestItemGatewayDB::clonedItemModificator_HasSameData()
{
  QVERIFY(mod_source->attribute(Attribute::DEX) != 0);
  QCOMPARE(mod_cloned->attribute(Attribute::DEX), mod_source->attribute(Attribute::DEX));
}

void TestItemGatewayDB::clonedItemWithInventory_HasInventory()
{
  insertItemIntoItemWithInventory(Item::forge(ItemPrototype::Nozyk));
  Item* cloned_item_with_inv = item_with_inventory->clone();

  QCOMPARE(cloned_item_with_inv->inventory()->count(), item_with_inventory->inventory()->count());
  QVERIFY(cloned_item_with_inv->inventory()->count() > 0);

  purgeInventory(item_with_inventory);
  purgeItemWithInventory(cloned_item_with_inv);
}

void TestItemGatewayDB::clonedItemWithInventory_HasValidItemCountInInventory()
{
  insertItemIntoItemWithInventory(Item::forge(ItemPrototype::BlankJewelry));
  insertItemIntoItemWithInventory(Item::forge(ItemPrototype::BlankOrdinary));

  Item* cloned_item_with_inv = item_with_inventory->clone();

  QCOMPARE(cloned_item_with_inv->inventory()->count(), item_with_inventory->inventory()->count());
  QCOMPARE(cloned_item_with_inv->inventory()->count(), (size_t)2);

  purgeInventory(item_with_inventory);
  purgeItemWithInventory(cloned_item_with_inv);
}

void TestItemGatewayDB::clonedInventoryItems_HaveDifferentId_ThanSourceItems()
{
  Item* inv_item = Item::forge(ItemPrototype::BlankTool);
  dbRef inv_item_ref = inv_item->ref();

  QVERIFY(inv_item_ref > 0);
  insertItemIntoItemWithInventory(inv_item);

  Item* cloned_item_with_inv = item_with_inventory->clone();
  Item* cloned_inv_item = cloned_item_with_inv->inventory()->get(0).item.get();

  QVERIFY(cloned_inv_item->ref() > 0);
  QVERIFY(cloned_inv_item->ref() != inv_item_ref);

}

void TestItemGatewayDB::fetchedDataInto_givestemWithValidData()
{
  Item* newItem = Item::create(2);
  itemGateway.fetchInto(newItem, 1);

  QVERIFY(newItem != nullptr);

  QCOMPARE(newItem->ref(), (dbRef)1);
  QCOMPARE(newItem->value(), 43);
  QCOMPARE(newItem->attack(), 2);
  QCOMPARE(newItem->damageReduction().bashing, 0);

  delete newItem;
}
