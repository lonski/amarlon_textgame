#ifndef TESTBODYPART_H
#define TESTBODYPART_H

#include "testsuite.h"
#include <QtTest/QtTest>

class TestBodyPart : public QTestSuite
{
  Q_OBJECT
public:
  TestBodyPart();
private slots:
  void bodypart_tostr();
  void bodypart_fromstr();
  void bodypart_equip();
  void bodypart_unequip();
  void bodypart_creation_fromstr();
};

#endif // TESTBODYPART_H
