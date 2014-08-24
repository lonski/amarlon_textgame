#ifndef ITEMCONTAINERSGATEWAYDB_H
#define ITEMCONTAINERSGATEWAYDB_H

#include "../itemcontainersgateway.h"

class ItemContainersGatewayDB : public ItemContainersGateway
{
public:
  ItemContainersGatewayDB();

protected:
  virtual bool containerExistsInDataSource(dbRef id);
  virtual MapRow getContainerDataFromDataSource(dbRef id);

};

#endif // ITEMCONTAINERSGATEWAYDB_H
