#ifndef CREATURE_GATEWAY_H
#define CREATURE_GATEWAY_H

#include "data_gateway.h"

class CreatureGateway : public DataGateway
{
public:
  CreatureGateway();      
  virtual ~CreatureGateway() {}

  virtual DBObject* fetch(dbRef id);
  virtual void fetchInto(DBObject* obj, dbRef id);
  virtual unsigned int write(DBObject* obj);
  virtual DBObject* clone(DBObject* to_clone);

};

#endif // CREATURE_GATEWAY_H
