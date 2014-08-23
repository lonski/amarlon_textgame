#ifndef ITEMS_GATEWAY_DB_H
#define ITEMS_GATEWAY_DB_H

#include "Include/data_gateways/itemsgateway.h"

class Item;

class ItemsGatewayDB : public ItemsGateway
{
public:
  ItemsGatewayDB();
  virtual ~ItemsGatewayDB() {}

protected:
  virtual MapRow getItemDataFromDataSource(unsigned int item_id);
  virtual std::vector<unsigned int> getItemModificatorIdsFromDataSource(unsigned int item_id);
  virtual bool itemExistsInDataSource(unsigned int item_id);
  virtual unsigned int writeItemDataToDataSource(Item* item);
  virtual unsigned int getNewItemId();

private:
  void updateItemInDataSource(Item *item);
  void insertItemIntoDataSource(Item *item);

};

#endif // ITEMS_GATEWAY_DB_H
