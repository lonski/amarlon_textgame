#include "item_containers_gateway_db.h"
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

dbRef ItemContainersGatewayDB::writeItemDataToDataSource(Item::Container *container)
{
  try
  {
    if (containerExistsInDataSource(container->ref()))
      updateContainerInDataSource(container);
    else
      insertContainerIntoDataSource(container);
  }catch(soci::soci_error &e)
  {
    qDebug() << e.what();
    qDebug() << _Database.get_last_query().c_str();
  }

  return container->ref();
}

void ItemContainersGatewayDB::updateContainerInDataSource(Item::Container *container)
{
  stringstream save_query;
  save_query << "UPDATE item_containers SET"
             << "  MAX_WEIGHT=" << container->weight_capacity().max
             << " ,NAME='" << container ->name()<< "'"
             << " ,OREF=" << container->oref()
             << " ,OTABLE='" << container->otable() << "'"
             << " ,ITEMS='" << container->getItemsString() << "'"
             << " WHERE ref=" << container->ref();

  _Database << save_query.str();
  _Database.commit();
}

void ItemContainersGatewayDB::insertContainerIntoDataSource(Item::Container *container)
{
  container->setRef(getNewContainerId());

  stringstream save_query;

  save_query <<
    " INSERT INTO item_containers (REF, MAX_WEIGHT, NAME, OREF, OTABLE, ITEMS) "
     << "VALUES("
     << container->ref()
     << "," << container->weight_capacity().max
     << "," << "'" << container->name() << "'"
     << "," << container->oref()
     << "," << "'" << container->otable() << "'"
     << "," << "'" << container->getItemsString() << "'"
     << ")";

  _Database << save_query.str();
  _Database.commit();
}
