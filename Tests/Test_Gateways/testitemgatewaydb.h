#ifndef TESTITEMGATEWAYDB_H
#define TESTITEMGATEWAYDB_H

#include <QtTest/QtTest>

#include "testsuite.h"
#include "Include/data_gateways/db_gateways/items_gateway_db.h"

class TestItemGatewayDB : public QTestSuite
{
  Q_OBJECT
public:
  TestItemGatewayDB();

private:
  ItemsGatewayDB itemGateway;

private slots:
  void fetchNonExistingOjects_returnsNull();
  void fetchExistingObject_returnsItem();
  void fetchedItemHasValidData();
  void updateingExistingItemInDataSource();
  void insertNewItemIntoDataSource();
};

#endif // TESTITEMGATEWAYDB_H
