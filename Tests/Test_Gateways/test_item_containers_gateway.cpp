#include "test_item_containers_gateway.h"
#include "Include/data_gateways/db_gateways/item_containers_gateway_db.h"

TestItemContainersGatewayDB::TestItemContainersGatewayDB()
{
  gateway = new ItemContainersGatewayDB;
}

TestItemContainersGatewayDB::~TestItemContainersGatewayDB()
{
}

void TestItemContainersGatewayDB::fetchId0_createsNewContainer()
{
  QVERIFY(gateway->fetch(0) != nullptr);
}

void TestItemContainersGatewayDB::fetchValidId_givesNonNullContainer()
{
  QVERIFY(gateway->fetch(1) != nullptr);
}

void TestItemContainersGatewayDB::fetchedContainer_hasValidData()
{
  Item::Container* container = dynamic_cast<Item::Container*>(gateway->fetch(1));
  assert_ContainerHasRef1Data(container);
}

void TestItemContainersGatewayDB::fetchIntoNullContainer_givesNull()
{
  Item::Container *nullCont = nullptr;
  gateway->fetchInto(nullCont, 1);

  QVERIFY(nullCont == nullptr);
}

void TestItemContainersGatewayDB::fetchIntoRawNewContainerIdZero_doesNotChangeContainer()
{
  Item::Container* rawNewContainer = new Item::Container();
  gateway->fetchInto(rawNewContainer, 0);

  QCOMPARE(rawNewContainer->name().c_str(), "");
  QCOMPARE(rawNewContainer->ref(), (dbRef)0);
}

void TestItemContainersGatewayDB::fetchedContainerIntoRawContainer_givesContainerWithValidData()
{
  Item::Container* rawNewContainer = new Item::Container();
  gateway->fetchInto(rawNewContainer, 1);

  assert_ContainerHasRef1Data(rawNewContainer);

  delete rawNewContainer;
}

void TestItemContainersGatewayDB::writeBaseContainerData_isPernament()
{
  Item::Container* container = dynamic_cast<Item::Container*>(gateway->fetch(1));

  changeItemDataForSomeRandomValues(container);
  gateway->write(container);

  delete container;
  container = dynamic_cast<Item::Container*>(gateway->fetch(1));

  QCOMPARE(container->name().c_str(), "changedName");
  QCOMPARE(container->ref(), (dbRef)1);
  QCOMPARE(container->weight_capacity().max, 69.0);
  QCOMPARE(container->weight_capacity().cur, 0.0);
  QCOMPARE(container->oref(), (dbRef)5);
  QCOMPARE(container->otable().c_str(), "newTable");

  setRef1Data(container);
  gateway->write(container);
  delete container;
}

void TestItemContainersGatewayDB::writeContainerThatDoesntExists_createsNewRecord()
{
  Item::Container* container = dynamic_cast<Item::Container*>(gateway->fetch(1));
  container->setRef(1111);

  dbRef r = gateway->write(container);
  delete container;

  QVERIFY(r != 1);
  QVERIFY(r != 1111);

  container = dynamic_cast<Item::Container*>(gateway->fetch(r));
  assert_ContainerHasRef1Data(container, r);

  delete container;
}

void TestItemContainersGatewayDB::changeItemDataForSomeRandomValues(Item::Container* container)
{
  container->setName("changedName");
  container->setORef(5);
  container->setOTable("newTable");
  container->set_max_weight(69);
}

void TestItemContainersGatewayDB::setRef1Data(Item::Container* container)
{
  container->setName("Ekwipunek");
  container->setORef(0);
  container->setOTable("");
  container->set_max_weight(10);
}

void TestItemContainersGatewayDB::assert_ContainerHasRef1Data(Item::Container* container, dbRef ref)
{
  QCOMPARE(container->name().c_str(), "Ekwipunek");
  QCOMPARE(container->ref(), ref);
  QCOMPARE(container->weight_capacity().max, 10.0);
  QCOMPARE(container->weight_capacity().cur, 0.0);
}
