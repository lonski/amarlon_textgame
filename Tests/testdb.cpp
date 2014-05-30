#include "testdb.h"

using namespace std;
using namespace soci;

TestDB::TestDB()
{
}

void TestDB::TestConnection()
{
  indicator ind;
  dbRef ref(0);
  DB::Session() << "select first 1 ref from locations", into(ref,ind);

  QVERIFY(ind == i_ok);
  QVERIFY(ref != 0);
}
