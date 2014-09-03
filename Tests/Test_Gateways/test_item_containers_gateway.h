#ifndef TESTITEMCONTAINERSGATEWAY_H
#define TESTITEMCONTAINERSGATEWAY_H

#include <QtTest/QtTest>

#include "testsuite.h"
#include "Equipment/item_container.h"

class ItemContainersGateway;

class TestItemContainersGatewayDB : public QTestSuite
{
  Q_OBJECT
public:
  TestItemContainersGatewayDB();
  ~TestItemContainersGatewayDB();

  void changeItemDataForSomeRandomValues(Item::Container* container);
  void setRef1Data(Item::Container* container);
  void assert_ContainerHasRef1Data(Item::Container* container, dbRef ref = 1);
private:
  ItemContainersGateway* gateway;

private slots:
  void fetchId0_createsNewContainer();
  void fetchValidId_givesNonNullContainer();
  void fetchedContainer_hasValidData();
  void fetchIntoNullContainer_givesNull();
  void fetchIntoRawNewContainerIdZero_doesNotChangeContainer();
  void fetchedContainerIntoRawContainer_givesContainerWithValidData();
  void writeBaseContainerData_isPernament();
  void writeContainerThatDoesntExists_createsNewRecord();
};

#endif // TESTITEMCONTAINERSGATEWAY_H
