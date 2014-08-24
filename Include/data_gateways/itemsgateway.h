#ifndef ITEMSGATEWAY_H
#define ITEMSGATEWAY_H

#include "Include/data_gateways/data_gateway.h"
#include "Include/db.h"

#include "Include/typedefs/def_db_item.h"

class Item;

class ItemsGateway : public DataGateway
{
public:
  ItemsGateway();
  virtual ~ItemsGateway() {}

  virtual DBObject* fetch(dbRef id);
  virtual void fetchInto(DBObject* obj);
  virtual unsigned int write(DBObject *obj);
  virtual DBObject* clone(DBObject *to_clone);

protected:
  virtual MapRow getItemDataFromDataSource(unsigned int item_id) = 0;
  virtual std::vector<unsigned int> getItemModificatorIdsFromDataSource(unsigned int item_id) = 0;
  virtual bool itemExistsInDataSource(unsigned int item_id) = 0;
  virtual unsigned int  writeItemDataToDataSource(Item* item) = 0;
  virtual unsigned int getNewItemId() = 0;

private:
  void readDataIntoItem(Item* item);

  void setItemModificators(Item *item);
  void setItemInventory(Item *item);
  void setItemData(MapRow item_data, Item *item);
  void setOrdinaryItemData(Item *item, MapRow item_data);
  void setWeaponItemData(Item *item, MapRow item_data);
  void setArmorItemData(Item *item, MapRow item_data);
  void setFoodItemData(MapRow item_data, Item *item);

  void writeItemInventory(Item* item);
  void writeItemCrtModificators(Item* item);

  void copyItemBaseData(Item* cloned, Item* item_to_clone);
  void copyBodyParts(Item* cloned, Item* item_to_clone);
  void copyCreatureModificators(Item* cloned, Item* item_to_clone);
  void copyInventory(Item* cloned, Item* item_to_clone);
  Item* createNewItemInstanceAndDBRecord();
};


#endif // ITEMSGATEWAY_H
