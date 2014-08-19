#ifndef ITEMS_GATEWAY_DB_H
#define ITEMS_GATEWAY_DB_H

#include "Include/data_gateways/data_gateway.h"
#include "Include/db.h"

class ItemsGatewayDB : public DataGateway
{
public:
  ItemsGatewayDB();
  virtual ~ItemsGatewayDB() {}

  virtual DBObject* fetch(int id);
  virtual void write(DBObject *obj);

};

#endif // ITEMS_GATEWAY_DB_H
