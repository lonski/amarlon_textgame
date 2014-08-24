#ifndef TESTCREATUREMODIFICATOR_H
#define TESTCREATUREMODIFICATOR_H

#include "testsuite.h"

#include <QtTest/QtTest>

class TestCreatureModificator : public QTestSuite
{
  Q_OBJECT
public:
  TestCreatureModificator();
private slots:
  void modificator_creation();
  void modificator_creation_existing();
  void modificator_load();
  void modificator_save();
  void modificator_augument();

};

#endif // TESTCREATUREMODIFICATOR_H
