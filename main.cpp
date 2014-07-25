#include <QApplication>
#include <QDebug>
#include "game.h"

//#define _UT

#ifdef _UT
#include <QtTest/QTest>
#include "Tests/testlocation.h"
#include "Tests/testdb.h"
#include "Tests/testitems.h"
#include "Tests/testfun.h"
#include "Tests/testcreature.h"
#include "Tests/testcommands.h"
#endif

int main(int argc, char *argv[])
{
  QApplication amarlon(argc, argv);

#ifdef _UT
  //TESTS
  TestDB db_tests;
  TestLocation location_tests;
  TestItems item_tests;
  TestFun fun_tests;
  TestCreature crt_tests;
  TestCommands cmd_tests;

  //TEST EXECUTION
  try
  {
    QTest::qExec(&db_tests, argc, argv);
    QTest::qExec(&location_tests, argc, argv);
    QTest::qExec(&item_tests, argc, argv);
    QTest::qExec(&fun_tests, argc, argv);
    QTest::qExec(&crt_tests, argc, argv);
    QTest::qExec(&cmd_tests, argc, argv);
  }
  catch(soci::soci_error &e)
  {
    qDebug() << e.what();
    qDebug() << _Database.get_last_query().c_str();
  }
  catch(std::exception &e)
  {
    qDebug() << e.what();
    qDebug() << _Database.get_last_query().c_str();
  }
#endif

  //START GAME
  _Game->show();
  _Game->console()->clear();
  _Console->handle_player_input("menu");

  return amarlon.exec();

}
