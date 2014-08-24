#include "testitemcontainersgateway.h"
#include "Include/data_gateways/db_gateways/itemcontainersgatewaydb.h"
#include "Equipment/item_container.h"

TestItemContainersGatewayDB::TestItemContainersGatewayDB()
{
  gateway = new ItemContainersGatewayDB;
}

TestItemContainersGatewayDB::~TestItemContainersGatewayDB()
{
}

void TestItemContainersGatewayDB::fetchId0_givesNullptr()
{
  QVERIFY(gateway->fetch(0) == nullptr);
}

void TestItemContainersGatewayDB::fetchValidId_givesNonNullContainer()
{
  QVERIFY(gateway->fetch(1) != nullptr);
}

void TestItemContainersGatewayDB::fetchedContainer_hasValidData()
{
  Item::Container* container = dynamic_cast<Item::Container*>(gateway->fetch(1));
  QCOMPARE(container->name().c_str(), "Ekwipunek");
  QCOMPARE(container->ref(), (dbRef)1);
  QCOMPARE(container->weight_capacity().max, 10.0);
  QCOMPARE(container->weight_capacity().cur, 0.0);
}
