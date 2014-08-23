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

private:
  ItemsGatewayDB itemGateway;
  Item* item;
  Item *item_to_clone;
  Item *cloned;
  CreatureModificator *mod_cloned;
  CreatureModificator *mod_source;

private slots:
  void fetchNonExistingOjects_returnsNull();
  void fetchExistingObject_returnsItem();
  void fetchedItemHasValidData();
  void updateingExistingItemInDataSource();
  void insertNewItemIntoDataSource();
  void clonedItemIsValidPointer();
  void clonedItemHasValidId();
  void clonedItemHasSameData();
  void clonedItemHasSameBodyParts();
  void clonedItemHasSameModificatorsCount();
  void clonedItemModificatorHasValidRef();
  void clonedItemModificatorHasValidOwner();
  void clonedItemModificatorHasSameData();
};

#endif // TESTITEMGATEWAYDB_H
