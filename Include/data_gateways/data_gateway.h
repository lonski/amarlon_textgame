#ifndef DATA_GATEWAY_H
#define DATA_GATEWAY_H

#include "Include/typedefs/def_db_item.h"

class DBObject;

class DataGateway
{
public:
  DataGateway() {}
  virtual ~DataGateway() {}

  virtual DBObject* fetch(dbRef id) = 0;
  virtual unsigned int write(DBObject* obj) = 0;
  virtual void fetchInto(DBObject* obj) = 0;
};

#endif // DATA_GATEWAY_H
