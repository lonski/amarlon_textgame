#include "creature_gateway_db.h"
#include "Include/db.h"
#include "Include/functions/db_utils.h"
#include "Creatures/creature.h"
#include "World/location.h"

using namespace soci;
using namespace fun;
using namespace std;

CreatureGatewayDB::CreatureGatewayDB()
{
}

bool CreatureGatewayDB::creatureExistsInDataSource(dbRef id)
{
  short r = 0;
  soci::indicator ind;
  _Database << "select 1 from creatures where ref=" << id, into(r, ind);

  return r;
}

MapRow CreatureGatewayDB::getCreatureDataFromDataSource(dbRef id)
{
  MapRow row;

  try
  {
    row = MapQuery( "SELECT * FROM creatures WHERE ref="+to_string(id) );
  }
  catch(soci_error &e)
  {
    qDebug() << e.what();
    qDebug() << _Database.get_last_query().c_str();
  }

  return row;
}

unsigned int CreatureGatewayDB::writeCreatureDataToDataSource(Creature *creature)
{
  try
  {
    if (creatureExistsInDataSource(creature->ref()))
      updateCreatureInDataSource(creature);
    else
      insertCreatureIntoDataSource(creature);
  }catch(soci::soci_error &e)
  {
    qDebug() << e.what();
    qDebug() << _Database.get_last_query().c_str();
  }

  return creature->ref();
}

void CreatureGatewayDB::updateCreatureInDataSource(Creature *creature)
{
  stringstream save_query;

  save_query << "UPDATE creatures SET "
             << "  NAME='" << creature->name() << "'"
             << ", DESCRIPT='" << creature->descript() << "'"
             << ", LOC_DESCRIPT='" << creature->locDescript() << "'"
             << ", SEX=" << static_cast<int>(creature->sex())
             << ", ATTRIBUTES='" << creature->stats().attributes2str() << "'"
             << ", SKILLS='" << creature->stats().skills2str() << "'"
             << ", BODY='" << creature->body().toStr() << "'"
             << ", LOCATION=" << (creature->getLocation() == nullptr ? 0 : creature->getLocation()->ref())
             << " WHERE ref = " << creature->ref();

  _Database << save_query.str();
  _Database.commit();
}

void CreatureGatewayDB::insertCreatureIntoDataSource(Creature *creature)
{
  creature->setRef(getNewCreatureId());

  stringstream save_query;

  save_query <<
    " INSERT INTO creatures (REF, NAME, DESCRIPT, LOC_DESCRIPT, SEX, ATTRIBUTES, SKILLS, BODY, LOCATION)"
     << "VALUES("
     << creature->ref()
     << "," << "'" << creature->name() << "'"
     << "," << "'" << creature->descript() << "'"
     << "," << "'" << creature->locDescript() << "'"
     << "," << static_cast<int>(creature->sex())
     << "," << "'" << creature->stats().attributes2str() << "'"
     << "," << "'" << creature->stats().skills2str() << "'"
     << "," << "'" << creature->body().toStr() << "'"
     << "," << (creature->getLocation() == nullptr ? 0 : creature->getLocation()->ref())
     << ")";

  _Database << save_query.str();
  _Database.commit();
}

unsigned int CreatureGatewayDB::getNewCreatureId()
{
  unsigned int new_ref = 0;
  soci::indicator ind;
  _Database << "select new_ref from GENERATE_CREATURE_REF", into(new_ref, ind);

  return new_ref;
}
