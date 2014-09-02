#ifndef TESTITEMCONTAINERSGATEWAY_H
#define TESTITEMCONTAINERSGATEWAY_H

#include <QtTest/QtTest>

#include "testsuite.h"

class ItemContainersGateway;

class TestItemContainersGatewayDB : public QTestSuite
{
  Q_OBJECT
public:
  TestItemContainersGatewayDB();
  ~TestItemContainersGatewayDB();

private:
  ItemContainersGateway* gateway;

private slots:
  void fetchId0_createsNewContainer();
  void fetchValidId_givesNonNullContainer();
  void fetchedContainer_hasValidData();
  void fetchedContainerIntoNullContainer_givesContainerWithValidData();
};

#endif // TESTITEMCONTAINERSGATEWAY_H
