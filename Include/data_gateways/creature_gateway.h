#ifndef CREATURE_GATEWAY_H
#define CREATURE_GATEWAY_H

#include "data_gateway.h"
#include "Include/typedefs/def_db_table.h"

class Creature;

class CreatureGateway : public DataGateway
{
public:
  CreatureGateway();      
  virtual ~CreatureGateway() {}

  virtual DBObject* fetch(dbRef id);
  virtual void fetchInto(DBObject* obj, dbRef id);
  virtual unsigned int write(DBObject* obj);
  virtual DBObject* clone(DBObject* to_clone);

  void setCreatureInventory(Creature *creature);
  void setCreatureMods(Creature *creature);

protected:
  virtual bool creatureExistsInDataSource(dbRef id) = 0;
  virtual MapRow getCreatureDataFromDataSource(dbRef id) = 0;
  virtual unsigned int  writeCreatureDataToDataSource(Creature* creature) = 0;

private:
  void readDataIntoCreature(Creature* creature);
  void setCreatureBaseData(Creature *creature, MapRow crt_data);
  void setCreatureStats(Creature *creature, MapRow crt_data);
  void setCreatureBody(MapRow crt_data, Creature *creature);

};

#endif // CREATURE_GATEWAY_H
