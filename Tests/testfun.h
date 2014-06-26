#ifndef TESTFUN_H
#define TESTFUN_H

#include <QtTest/QtTest>
#include "../Include/db.h"
#include "../Include/func.h"
#include "../Include/gameclock.h"

class TestFun : public QObject
{
  Q_OBJECT
public:
  TestFun() {}
private slots:
   void skill_group();
   void explode();
   void gameclock();
};

#endif // TESTFUN_H
