#ifndef TESTDB_H
#define TESTDB_H
#include <QtTest/QtTest>
#include "../Include/db.h"

class TestDB : public QObject
{
  Q_OBJECT
public:
  TestDB();

private:
  void setDBConnectionLocalServer();
  void setDBConnectionRemote();
  void setDBConnectionLocalMachine();

private slots:
   void TestConnection();
};

#endif // TESTDB_H
