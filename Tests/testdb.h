#ifndef TESTDB_H
#define TESTDB_H
#include <QtTest/QtTest>
#include "../Include/common.h"

class TestDB : public QObject
{
  Q_OBJECT
public:
  TestDB();
private slots:
   void TestConnection();
};

#endif // TESTDB_H
