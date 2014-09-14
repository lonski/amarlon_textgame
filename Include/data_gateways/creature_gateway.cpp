#include "creature_gateway.h"
#include "Creatures/creature.h"
#include "Equipment/item_container.h"

using namespace std;
using namespace fun;

CreatureGateway::CreatureGateway()
{
}

DBObject *CreatureGateway::fetch(dbRef id)
{
  Creature* creature = nullptr;

  if (creatureExistsInDataSource(id))
  {
    creature = new Creature(id);
    readDataIntoCreature(creature);
  }

  return creature;
}

void CreatureGateway::fetchInto(DBObject *obj, dbRef id)
{
  Creature* creature = dynamic_cast<Creature*>(obj);

  if (creature != nullptr && creatureExistsInDataSource(id))
  {
    creature->setRef(id);
    readDataIntoCreature(creature);
  }
}

unsigned int CreatureGateway::write(DBObject *obj)
{
  Creature* crt = dynamic_cast<Creature*>(obj);
  unsigned int r = 0;

  if (crt != nullptr)
  {
    r = writeCreatureDataToDataSource(crt);
  }

  return r;
}

DBObject *CreatureGateway::clone(DBObject *to_clone)
{
  Creature* creatureToClone = dynamic_cast<Creature*>(to_clone);
  Creature* creatureCloned = dynamic_cast<Creature*>(fetch(0));
  write(creatureCloned);

  creatureCloned->setName(creatureToClone->name());
  creatureCloned->setDescript(creatureToClone->descript());
  creatureCloned->setLocDescript(creatureToClone->locDescript());
  creatureCloned->setSex(creatureToClone->sex());
  creatureCloned->stats().str2attributes(creatureToClone->stats().attributes2str());
  creatureCloned->stats().str2skills(creatureToClone->stats().skills2str());
  creatureCloned->body().load(creatureToClone->body().toStr());
  //TODO mods: from item gateway
  //TODO inventory: from item gateway

  return creatureCloned;
}

void CreatureGateway::readDataIntoCreature(Creature *creature)
{
  if ( !creature->loaded() && creature->ref() > 0 )
  {
    MapRow crt_data = getCreatureDataFromDataSource(creature->ref());

    setCreatureBaseData(creature, crt_data);
    setCreatureStats(creature, crt_data);
    setCreatureBody(crt_data, creature);
    setCreatureMods(creature);
    setCreatureInventory(creature);

    creature->calcTotalDamage();
    creature->calcWeapons();
    creature->set_loaded();
    creature->set_not_modified();
  }
}

void CreatureGateway::setCreatureBaseData(Creature *creature, MapRow crt_data)
{
  creature->setName( CheckValue<string>(crt_data["NAME"]) );
  creature->setDescript( CheckValue<string>(crt_data["DESCRIPT"]) );
  creature->setLocDescript( CheckValue<string>(crt_data["LOC_DESCRIPT"]) );
  creature->setSex( CheckValueCast<Sex>(crt_data["SEX"]));
}

void CreatureGateway::setCreatureStats(Creature *creature, MapRow crt_data)
{
  creature->stats().str2attributes(crt_data["ATTRIBUTES"]);
  creature->stats().str2skills( CheckValue<string>(crt_data["SKILLS"]) );
}

void CreatureGateway::setCreatureBody(MapRow crt_data, Creature *creature)
{
  creature->body().load(CheckValue<string>(crt_data["BODY"]));
}

void CreatureGateway::setCreatureInventory(Creature *creature)
{
  Item::Container* inventory = nullptr;
  dbRef inv_ref = Item::Container::byOwner( "creatures", creature->ref() );
  if (inv_ref != 0)
  {
    inventory = Item::Container::create(inv_ref);
  }
  creature->setInventory(inventory);
}

void CreatureGateway::setCreatureMods(Creature *creature)
{
  //MODS TODO: loading mods conected to creature
  //zaladuj z crt_mods
  //vector<int> mod_refs(100);
  //vector<indicator> inds;

  //string query = "SELECT ref FROM crt_mods WHERE otable='" + table() + "' and oref=" + fun::toStr(ref());
  //_Database << query, into(mod_refs, inds);

  //for (auto m = mod_refs.begin(); m != mod_refs.end(); ++m)
  //  _mods->add( shared_ptr<CreatureModificator>(new CreatureModificator(*m)) );

  //dodaj z itemów założonych na bodyParts
  //TODO: refactor: law of demeter violation
  for (auto im = creature->body().equipped_items().begin(); im != creature->body().equipped_items().end(); ++im)
  {
    if (nullptr != *im && !(*im)->mods()->getAll().empty())
      creature->mods()->add( (*im)->mods()->get_complex_mod() );
  }
}
