#include <QtTest/QtTest>
#include <iostream>

#include "testsuite.h"

#include "Test_DB/testdb.h"
#include "Test_Commands/testcommands.h"
#include "Test_Creature/testcreature.h"
#include "Test_Functions/testfun.h"
#include "Test_Items/testitems.h"
#include "Test_Location/testlocation.h"

static TestDB tdb;
static TestCommands tcmd;
static TestCreature tcrt;
static TestFun tfun;
static TestItems titm;
static TestLocation tloc;

int main(int, char**)
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
