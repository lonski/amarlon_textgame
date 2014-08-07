
#include "testdb.h"

using namespace std;
using namespace soci;

TestDB::TestDB()
{
}

void TestDB::setDBConnectionLocalServer()
{
  string db_file = "/home/pi/db/data.fdb";
  string db_log_file = "../amarlon/Data/db.log";
  string db_server = "192.168.1.5";//"lonski.pl";

  DB::SetDatabaseInfo(db_file, db_server, db_log_file);
}

void TestDB::setDBConnectionRemote()
{
  string db_file = "/home/pi/db/data.fdb";
  string db_log_file = "../amarlon/Data/db.log";
  string db_server = "lonski.pl";

  DB::SetDatabaseInfo(db_file, db_server, db_log_file);
}

void TestDB::setDBConnectionLocalMachine()
{
  string db_file = "/home/spszenguo/Projects/amarlon/Data/data2.fdb";
  string db_log_file = "../amarlon/Data/db.log";
  string db_server = "localhost";

  DB::SetDatabaseInfo(db_file, db_server, db_log_file);
}


void TestDB::TestConnection()
{
  setDBConnectionLocalServer();
  indicator ind;
  dbRef ref(0);
  DB::Session() << "select first 1 ref from locations", into(ref,ind);

  QVERIFY(ind == i_ok);
  QVERIFY(ref != 0);
}

