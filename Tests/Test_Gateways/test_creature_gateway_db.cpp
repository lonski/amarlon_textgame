#include "test_creature_gateway_db.h"
#include "Include/data_gateways/db_gateways/creature_gateway_db.h"
#include "Creatures/creature.h"
#include "Include/enums/e_refdict.h"

TestCreatureGatewayDB::TestCreatureGatewayDB()
{
  gateway = new CreatureGatewayDB;
  existingCreature = dynamic_cast<Creature*>(gateway->fetch((int)refDict::Creature::Ogr_Mateusz));
}

TestCreatureGatewayDB::~TestCreatureGatewayDB()
{
  delete existingCreature;
}

void TestCreatureGatewayDB::fetchNonExistingCreature_givesNull()
{
  Creature* crt = dynamic_cast<Creature*>(gateway->fetch(0));
  QVERIFY(crt == nullptr);
}

void TestCreatureGatewayDB::fetchExistingCreature_givesNotNull()
{
  QVERIFY(existingCreature != nullptr);
}

void TestCreatureGatewayDB::fetchExistingCreature_hasValidBaseData()
{
  QCOMPARE(existingCreature->ref(), (dbRef)refDict::Creature::Ogr_Mateusz);
  QCOMPARE(existingCreature->name().c_str(), "Ogr Mateusz");
  //other data validation are in TestCreature
}
