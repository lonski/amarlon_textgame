#include <QtTest/QtTest>
#include <iostream>

#include "testsuite.h"

#include "Test_DB/testdb.h"
#include "Test_Commands/testcommands.h"
#include "Test_Creature/testcreature.h"
#include "Test_Functions/testfun.h"
#include "Test_Items/testitems.h"
#include "Test_Location/testlocation.h"
#include "Test_Gateways/testitemgatewaydb.h"

int main(int, char**)
{
  //active tests:
  TestDB tdb;
  TestCommands tcmd;
  TestItems titm;
  TestCreature tcrt;
  TestFun tfun;
  TestLocation tloc;
  TestItemGatewayDB tgidb;
  //

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

  qDebug() << "Passed: " << QTestSuite::m_suites.size() - failedSuitesCount << "/" << QTestSuite::m_suites.size();
  return failedSuitesCount;
}
