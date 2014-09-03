#include "itemcontainersgateway.h"

using namespace std;

ItemContainersGateway::ItemContainersGateway()
{
}

ItemContainersGateway::~ItemContainersGateway()
{
}

DBObject *ItemContainersGateway::fetch(dbRef id)
{
  Item::Container* container = nullptr;

  if (containerExistsInDataSource(id))
  {
    container = new Item::Container(id);
    readDataIntoContainer(container);
  }
  else
  {
    dbRef newId = getNewContainerId();
    container = new Item::Container(newId);
    container->set_max_weight(Item::Container::defaultCapacity);

    container->set_loaded();
    container->set_not_modified();
  }

  return container;
}

void ItemContainersGateway::fetchInto(DBObject *obj, dbRef id)
{

}

unsigned int ItemContainersGateway::write(DBObject *obj)
{

}

DBObject *ItemContainersGateway::clone(DBObject *to_clone)
{

}

void ItemContainersGateway::readDataIntoContainer(Item::Container *container)
{
  if (!container->loaded() && container->ref() > 0)
  {
    MapRow container_data = getContainerDataFromDataSource(container->ref());

    setContainerParams(container, container_data);

    container->set_loaded();
    container->set_not_modified();
  }
}

void ItemContainersGateway::setContainerParams(Item::Container *container, MapRow container_data)
{
  container->setName( fun::CheckValue<std::string>(container_data["NAME"]) );
  container->set_max_weight( fun::CheckValue<Weight>(container_data["MAX_WEIGHT"]) );
  container->setORef( fun::CheckValue<dbRef>(container_data["OREF"]) );
  container->setOTable( fun::CheckValue<std::string>(container_data["OTABLE"]) );
  container->setItems( fun::CheckValue<string>(container_data["ITEMS"]));
}
