#include "testdb.h"

using namespace std;
using namespace soci;

TestDB::TestDB()
{
}

void TestDB::TestConnection()
{
  string db_file = "/home/pi/db/data.fdb";
  string db_log_file = "../amarlon/Data/db.log";
  string db_server = "192.168.1.5";//"lonski.pl";

  DB::SetDatabaseInfo(db_file, db_server, db_log_file);

  indicator ind;
  dbRef ref(0);
  DB::Session() << "select first 1 ref from locations", into(ref,ind);

  QVERIFY(ind == i_ok);
  QVERIFY(ref != 0);
}
