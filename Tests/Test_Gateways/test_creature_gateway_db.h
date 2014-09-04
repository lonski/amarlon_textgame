#ifndef TEST_CREATURE_GATEWAY_DB_H
#define TEST_CREATURE_GATEWAY_DB_H

#include <QtTest/QtTest>
#include "testsuite.h"

class DataGateway;
class Creature;

class TestCreatureGatewayDB : public QTestSuite
{
  Q_OBJECT
public:
  explicit TestCreatureGatewayDB();
  ~TestCreatureGatewayDB();

private:
  DataGateway* gateway;
  Creature* existingCreature;

private slots:
  void fetchNonExistingCreature_givesNull();
  void fetchExistingCreature_givesNotNull();
  void fetchExistingCreature_hasValidBaseData();

};

#endif // TEST_CREATURE_GATEWAY_DB_H
