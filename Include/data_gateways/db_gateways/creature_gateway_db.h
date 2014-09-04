#ifndef CREATURE_GATEWAY_DB_H
#define CREATURE_GATEWAY_DB_H

#include "../creature_gateway.h"

class CreatureGatewayDB : public CreatureGateway
{
public:
  CreatureGatewayDB();

protected:
  virtual bool creatureExistsInDataSource(dbRef id);
  virtual MapRow getCreatureDataFromDataSource(dbRef id);

};

#endif // CREATURE_GATEWAY_DB_H
