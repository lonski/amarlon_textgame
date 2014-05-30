#include <QtTest/QTest>
#include <QApplication>

#include "game.h"
#include "Tests/testlocation.h"
#include "Tests/testdb.h"
#include "Tests/testitems.h"

int main(int argc, char *argv[])
{
  //QApplication a(argc, argv);
  //Game w;
  //w.show();

  //TESTS
  TestDB db_tests;
  TestLocation location_tests;
  TestItems item_tests;

  //TEST EXECUTION
  QTest::qExec(&db_tests, argc, argv);
  QTest::qExec(&location_tests, argc, argv);
  QTest::qExec(&item_tests, argc, argv);

  //return a.exec();
  return 0;
}
