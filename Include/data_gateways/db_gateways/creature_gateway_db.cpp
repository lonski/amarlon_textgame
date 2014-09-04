#include "creature_gateway_db.h"
#include "Include/db.h"
#include "Include/functions/db_utils.h"

using namespace soci;
using namespace fun;
using namespace std;

CreatureGatewayDB::CreatureGatewayDB()
{
}

bool CreatureGatewayDB::creatureExistsInDataSource(dbRef id)
{
  short r = 0;
  soci::indicator ind;
  _Database << "select 1 from creatures where ref=" << id, into(r, ind);

  return r;
}

MapRow CreatureGatewayDB::getCreatureDataFromDataSource(dbRef id)
{
  MapRow row;

  try
  {
    row = MapQuery( "SELECT * FROM creatures WHERE ref="+to_string(id) );
  }
  catch(soci_error &e)
  {
    qDebug() << e.what();
    qDebug() << _Database.get_last_query().c_str();
  }

  return row;
}
