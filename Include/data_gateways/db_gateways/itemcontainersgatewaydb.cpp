#include "itemcontainersgatewaydb.h"
#include "Include/db.h"

using namespace soci;
using namespace std;

ItemContainersGatewayDB::ItemContainersGatewayDB()
{
}

bool ItemContainersGatewayDB::containerExistsInDataSource(dbRef id)
{
  short r = 0;
  soci::indicator ind;
  _Database << "select 1 from item_containers where ref=" << id, into(r, ind);

  return r;
}

MapRow ItemContainersGatewayDB::getContainerDataFromDataSource(dbRef id)
{
  MapRow cont_data;

  try
  {
    cont_data = fun::MapQuery("SELECT name, otable, oref, max_weight, items FROM item_containers WHERE ref="+to_string(id));
  }
  catch(soci_error &e)
  {
    qDebug() << e.what();
    qDebug() << _Database.get_last_query().c_str();
  }

  return cont_data;
}

unsigned int ItemContainersGatewayDB::getNewContainerId()
{
  unsigned int new_ref = 0;
  soci::indicator ind;
  _Database << "select new_ref from create_new_item_container", into(new_ref, ind);

  return new_ref;
}
