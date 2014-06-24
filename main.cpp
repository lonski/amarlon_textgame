#include <QtTest/QTest>
#include <QApplication>

#include "game.h"
#include "Tests/testlocation.h"
#include "Tests/testdb.h"
#include "Tests/testitems.h"
#include "Tests/testfun.h"
#include "Tests/testcreature.h"

int main(int argc, char *argv[])
{
  QApplication amarlon(argc, argv);

  //TESTS
  TestDB db_tests;
  TestLocation location_tests;
  TestItems item_tests;
  TestFun fun_tests;
  TestCreature crt_tests;

  //TEST EXECUTION
  //QTest::qExec(&db_tests, argc, argv);
  //QTest::qExec(&location_tests, argc, argv);
  //QTest::qExec(&item_tests, argc, argv);
  //QTest::qExec(&fun_tests, argc, argv);
  QTest::qExec(&crt_tests, argc, argv);

  //START GAME
  Game main_window;
  main_window.show();
  amarlon.exec();

  return 0;
}
