#ifndef TESTCREATURECONTAINER_H
#define TESTCREATURECONTAINER_H

#include "testsuite.h"

#include <QtTest/QtTest>
#include "../Include/db.h"

class TestCreatureContainer : public QTestSuite
{
  Q_OBJECT
public:
  TestCreatureContainer();
private slots:
  void creature_container_creation();
  void creature_container_load_save();
  void creature_container_insert_erase();
};

#endif // TESTCREATURECONTAINER_H
