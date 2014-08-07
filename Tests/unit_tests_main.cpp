#include <QtTest/QtTest>
#include <iostream>

#include "testsuite.h"

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
