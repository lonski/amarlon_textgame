#ifndef TESTITEMGATEWAYDB_H
#define TESTITEMGATEWAYDB_H

#include <QtTest/QtTest>

#include "testsuite.h"
#include "Include/data_gateways/db_gateways/items_gateway_db.h"

class CreatureModificator;

class TestItemGatewayDB : public QTestSuite
{
  Q_OBJECT
public:
  TestItemGatewayDB();
  ~TestItemGatewayDB();

  void purgeItemWithInventory(Item* cloned_item_with_inv);
private:
  ItemsGatewayDB itemGateway;
  Item* item;
  Item *item_to_clone;
  Item *cloned;
  Item *item_with_inventory;
  CreatureModificator *mod_cloned;
  CreatureModificator *mod_source;

  void insertItemIntoItemWithInventory(Item* item_to_insert);
  void purgeInventory(Item *item_with_inventory);

private slots:
  void fetchNonExistingOjects_returnsNull();
  void fetchExistingObject_returnsItem();
  void fetchedItem_HasValidData();
  void updateingExistingItemInDataSource();
  void insertNewItemIntoDataSource();
  void clonedItem_IsValidPointer();
  void clonedItem_HasValidId();
  void clonedItem_HasSameData();
  void clonedItem_HasSameBodyParts();
  void clonedItem_HasSameModificatorsCount();
  void clonedItemModificator_HasValidRef();
  void clonedItemModificator_HasValidOwner();
  void clonedItemModificator_HasSameData();
  void clonedItemWithInventory_HasInventory();
  void clonedItemWithInventory_HasValidItemCountInInventory();
  void clonedInventoryItems_HaveDifferentId_ThanSourceItems();
};

#endif // TESTITEMGATEWAYDB_H
