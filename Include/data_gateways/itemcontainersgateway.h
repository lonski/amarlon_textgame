#ifndef ITEMCONTAINERSGATEWAY_H
#define ITEMCONTAINERSGATEWAY_H

#include "Include/data_gateways/data_gateway.h"
#include "Include/typedefs/def_db_table.h"
#include "Equipment/item_container.h"

class ItemContainersGateway : public DataGateway
{
public:
  ItemContainersGateway();
  virtual ~ItemContainersGateway();

  virtual DBObject* fetch(dbRef id);
  virtual void fetchInto(DBObject* obj);
  virtual unsigned int write(DBObject *obj);
  virtual DBObject* clone(DBObject *to_clone);

protected:
  virtual bool containerExistsInDataSource(dbRef id) = 0;
  virtual MapRow getContainerDataFromDataSource(dbRef id) = 0;

private:
  void readDataIntoContainer(Item::Container* container);

};

#endif // ITEMCONTAINERSGATEWAY_H
