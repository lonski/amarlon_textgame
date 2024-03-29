#ifndef TESTFUN_H
#define TESTFUN_H

#include "testsuite.h"

#include <QtTest/QtTest>
#include "../Include/db.h"

#include "../Include/gameclock.h"
#include "Include/inifile.h"

class TestFun : public QTestSuite
{
  Q_OBJECT
public:
  TestFun() {}
private slots:
   void skill_group();
   void explode();
   void gameclock();
   void inifile();
   void temp();
};

#endif // TESTFUN_H
