#ifndef ITEMCONTAINERSGATEWAYDB_H
#define ITEMCONTAINERSGATEWAYDB_H

#include "../item_containers_gateway.h"

class ItemContainersGatewayDB : public ItemContainersGateway
{
public:
  ItemContainersGatewayDB();

protected:
  virtual bool containerExistsInDataSource(dbRef id);
  virtual MapRow getContainerDataFromDataSource(dbRef id);
  virtual unsigned int getNewContainerId();
  virtual dbRef writeItemDataToDataSource(Item::Container *container);

private:
  void updateContainerInDataSource(Item::Container* container);
  void insertContainerIntoDataSource(Item::Container* container);

};

#endif // ITEMCONTAINERSGATEWAYDB_H
