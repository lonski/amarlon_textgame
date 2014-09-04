#include "test_creature_gateway_db.h"
#include "Include/data_gateways/db_gateways/creature_gateway_db.h"

TestCreatureGatewayDB::TestCreatureGatewayDB()
{
}

void TestCreatureGatewayDB::gatewayCreation()
{
  DataGateway *gateway = new CreatureGatewayDB;
}
