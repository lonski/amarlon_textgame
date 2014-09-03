#include "testlocation.h"

using namespace std;

/* Loc connections (test map). Numbers are location's refs.

        7
        |
        6
      / | \
9 - 1 - 4 - 5
| x | x | /
8 - 2 - 3

*/

TestLocation::TestLocation()
{
}

void TestLocation::LocCreation()
{
  try
  {
    //create loc "Test1"
    Location *loc = new Location(1);

    //validate data
    QVERIFY(loc != nullptr);
    QCOMPARE(loc->ref(), static_cast<unsigned int>(1));
    QVERIFY(loc->name() == "");
    QVERIFY(loc->connection(Direction::South) == nullptr);
  }
  catch(error::creation_error &e)
  {
    qDebug() << "Error: " << e.what();
  }

  Location::Manager.purge();
}

void TestLocation::LoadingData()
{
  try
  {
    //create loc "Test3"
    Location *loc = new Location(3);
    QVERIFY(loc->loaded() == false);

    //then load it
    loc->load();
    QVERIFY(loc->loaded() == true);

    //and validate data
    QVERIFY(loc->drawn() == false);
    QVERIFY(loc->enterable() == true);
    QCOMPARE(loc->name().c_str(), "Test3");
    QCOMPARE(loc->descript().c_str(), "Testowa lokacja3");

    //vlidate one of neighbour's ptrs - location created but not yet loaded
    Location* nb = loc->connection(Direction::Northeast);
    QVERIFY(nb != nullptr);
    QVERIFY(nb->ref() == 5);
    QVERIFY(nb->name() == "");
    QVERIFY(nb->loaded() == false);

    //validate another neighbour ptr - still nullptr (no neighbour there)
    nb = loc->connection(Direction::East);
    QVERIFY(nb == nullptr);

  }
  catch(std::exception &e)
  {
    qDebug() << "Error: " << e.what();
  }

  Location::Manager.purge();
}

void TestLocation::LocWalkWithinRange()
{
  try
  {
    //create loc "Test5"
    Location *loc = new Location(5);

    //perform load-walk within the range of 1
    loc->locWalkWithinRange(WalkVector(1,1,1,1), &Location::loadNoParam);

    //validate data (look for loc map at begining of this file):
    //#1. locations of refs 6,4,3 should be loaded
    //Loc 6
    Location *loc6 =loc->connection(Direction::Northwest);
    QCOMPARE(loc6->ref(), static_cast<dbRef>(6));
    QVERIFY(loc6->loaded());
    //Loc 4
    Location *loc4 = loc->connection(Direction::West);
    QCOMPARE(loc4->ref(), static_cast<dbRef>(4));
    QVERIFY(loc4->loaded());
    //Loc 3
    Location *loc3 = loc->connection(Direction::Southwest);
    QCOMPARE(loc3->ref(), static_cast<dbRef>(3));
    QVERIFY(loc3->loaded());

    //#2. locations of refs 7,1,2 should be created
    //Loc 7
    Location *loc7 = loc6->connection(Direction::North);
    QVERIFY(loc7 != nullptr);
    QCOMPARE(loc7->ref(), static_cast<dbRef>(7));
    QVERIFY(loc7->loaded() == false);
    //Loc 1
    Location *loc1 = loc4->connection(Direction::West);
    QVERIFY(loc1 != nullptr);
    QCOMPARE(loc1->ref(), static_cast<dbRef>(1));
    QVERIFY(loc1->loaded() == false);
    //Loc 2
    Location *loc2 = loc4->connection(Direction::Southwest);
    QVERIFY(loc2 != nullptr);
    QCOMPARE(loc2->ref(), static_cast<dbRef>(2));
    QVERIFY(loc2->loaded() == false);

    //#3. locations of refs 9,8 should not be created
    //Loc 9
    Location *loc9 = loc1->connection(Direction::West);
    QVERIFY(loc9 == nullptr);
    //Loc 8
    Location *loc8 = loc2->connection(Direction::West);
    QVERIFY(loc8 == nullptr);

  }
  catch(std::exception &e)
  {
    qDebug() << "Error: " << e.what();
  }

  Location::Manager.purge();
}

void TestLocation::SaveData()
{
  //create loc "Test5"
  Location *loc = new Location(5);
  loc->load();

  //validate
  QCOMPARE(loc->name().c_str(), "Test5");

  //rename
  loc->setName("changed_name");

  //validate rename
  QCOMPARE(loc->name().c_str(), "changed_name");
  loc->saveToDB();
  loc->load();
  QCOMPARE(loc->name().c_str(), "changed_name");

  //rollback changes
  loc->setName("Test5");
  loc->saveToDB();

  Location::Manager.purge();
}