#ifndef TESTCREATUREMODIFICATORMANAGER_H
#define TESTCREATUREMODIFICATORMANAGER_H

#include "testsuite.h"
#include <QtTest/QtTest>

class TestCreatureModificatorManager : public QTestSuite
{
  Q_OBJECT
public:
  TestCreatureModificatorManager();
private slots:
  void modmanager();
  void modmanager_ticktime();

};

#endif // TESTCREATUREMODIFICATORMANAGER_H
