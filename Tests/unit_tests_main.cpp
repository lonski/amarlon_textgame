#include <QtTest/QtTest>
#include <iostream>

#include "testsuite.h"

#include "Test_DB/testdb.h"
#include "Test_Commands/testcommands.h"
#include "Test_Creature/testcreature.h"
#include "Test_Creature/testcreaturemodificatormanager.h"
#include "Test_Creature/testbodypart.h"
#include "Test_Creature/testcreaturecontainer.h"
#include "Test_Creature/testcreaturestats.h"
#include "Test_Creature/testcreaturemodificator.h"
#include "Test_Functions/testfun.h"
#include "Test_Items/testitems.h"
#include "Test_Location/testlocation.h"
#include "Test_Gateways/test_item_gateway_db.h"
#include "Test_Gateways/test_item_containers_gateway.h"
#include "Test_Gateways/test_creature_gateway_db.h"

void displayFinalTestStats(int failedSuitesCount)
{
  std::cout << "### Passed test suites: "
            << QTestSuite::m_suites.size() - failedSuitesCount
            << "/"
            << QTestSuite::m_suites.size()
            << std::endl;
}

void cleanUp()
{
  std::cout << "### Cleaning database..." << std::endl;
  _Database << "execute procedure clean";
  _Database.commit();
}

int runTestSuites()
{
  int failedSuitesCount = 0;
  std::vector<QObject*>::iterator iSuite;

  for (iSuite = QTestSuite::m_suites.begin(); iSuite != QTestSuite::m_suites.end(); ++iSuite)
  {
    int result = QTest::qExec(*iSuite);
    if (result != 0)
    {
      failedSuitesCount++;
    }
  }

  return failedSuitesCount;
}

int main(int, char**)
{
  //active test suites:
  TestDB tdb;
  TestCommands tcmd;
  TestItems titm;
  TestCreature tcrt;
  TestCreatureStats tcstats;
  TestCreatureModificator tcmod;
  TestCreatureModificatorManager tcmodmng;
  TestCreatureContainer tccont;
  TestBodyPart tbpart;
  TestFun tfun;
  TestLocation tloc;
  TestItemGatewayDB tgidb;
  TestItemContainersGatewayDB tgicdb;
  TestCreatureGatewayDB tcrtgdb;
  //

  int failedSuitesCount = runTestSuites();
  displayFinalTestStats(failedSuitesCount);
  cleanUp();

  return failedSuitesCount;
}
