#ifndef TEST_CREATURE_GATEWAY_DB_H
#define TEST_CREATURE_GATEWAY_DB_H

#include "testsuite.h"

class TestCreatureGatewayDB : public QTestSuite
{
  Q_OBJECT
public:
  explicit TestCreatureGatewayDB();

public slots:
  void gatewayCreation();

};

#endif // TEST_CREATURE_GATEWAY_DB_H
