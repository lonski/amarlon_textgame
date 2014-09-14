#include "creature.h"
#include "Equipment/item_container.h"
#include "creaturecontainer.h"
#include "World/location.h"
#include "Include/data_gateways/db_gateways/creature_gateway_db.h"

#include "Include/enums/e_creaturetype.h"
#include "Include/functions/messages.h"

using namespace std;
using namespace soci;
using namespace fun;

const dbTable Creature::tableName = "creatures";
const dbTable Creature::Container::tableName = "crt_containers";
CreatureManager Creature::Manager;
DataGateway* Creature::gateway = new CreatureGatewayDB;

Creature::Creature(dbRef ref)
  : DBObject(ref)
  , _sex(Sex::Null)
  , _mods(new CreatureModificatorManager(this))
  , _total_damage(DamageLevel::Brak)
  , _weapon(nullptr)
  , _offhand(nullptr)
  , _shield(nullptr)
  , _currentLoc(nullptr)
  , _prevLoc(nullptr)
{  
}

Creature::~Creature()
{
  saveToDB();
  Manager.remove(ref());
  delete _mods;
  _mods = nullptr;
}

void Creature::calcWeapons()
{
  _weapon = nullptr;
  _offhand = nullptr;
  _shield = nullptr;

  for ( auto p = _body.parts().begin(); p != _body.parts().end(); ++p )
    {
      BodyPart *bp = p->get();
      Item *eq_wpn = bp->equipped(ItemType::Weapon).lock().get();
      Item *eq_shd = bp->equipped(ItemType::Shield).lock().get();

      if (eq_wpn != nullptr)
        {
          if (bp->side() == BodySide::Left)
            {
              _offhand = eq_wpn;
            }
          else if (bp->side() == BodySide::Right)
            {
              _weapon = eq_wpn;
            }
        }
      else if (eq_shd != nullptr)
        {
          _shield = eq_shd;
        }
    }
}


Item::Inventory &Creature::inventoryContainer()
{
  if (_inventory == nullptr)
    {
      _inventory.reset(Item::Container::create());
      _inventory->setOTable(table());
      _inventory->setORef(ref());
      _inventory->saveToDB();
    }

  return _inventory;
}

Creature *Creature::create(dbRef ref, bool)
{
  Creature* new_crt = dynamic_cast<Creature*>(gateway->fetch(ref));
  Manager.add(new_crt);

  return new_crt;
}

Creature* Creature::clone()
{
  return dynamic_cast<Creature*>(gateway->clone(this));
}

void Creature::load(MapRow*)
{
  Creature::gateway->fetchInto(this, ref());
}

void Creature::saveToDB()
{
  gateway->write(this);
}

void Creature::purge()
{
  if (inventoryContainer() != nullptr) inventoryContainer()->purge();
  DBObject::purge();
}

string Creature::name() const
{
  return _name;
}

string Creature::locDescript() const
{
  return _locDescript;
}

string Creature::descript() const
{
  return _descript;
}

Sex Creature::sex() const
{
  return _sex;
}

DamageLevel Creature::totalDamage() const
{
  return _total_damage;
}

int Creature::attribute(Attribute atr) const
{
  return _stats.attribute(atr) + _mods->get_complex_mod()->creature_stats().attribute(atr);
}

int Creature::skill(Skill skill) const
{
  return _stats.skill(skill) + _mods->get_complex_mod()->creature_stats().skill(skill);
}

CreatureStats &Creature::stats()
{
  return _stats;
}

Location *Creature::getLocation() const
{
  return _currentLoc;
}

Location *Creature::getPrevLoc() const
{
  return _prevLoc;
}

CreatureModificatorManager *Creature::mods()
{
  return _mods;
}

Body &Creature::body()
{
  set_modified();
  return _body;
}

void Creature::setName(string name)
{
  _name = name;
  set_modified();
}

void Creature::setDescript(string descript)
{
  _descript = descript;
  set_modified();
}

void Creature::setLocDescript(string locDescript)
{
  _locDescript = locDescript;
  set_modified();
}

void Creature::setSex(Sex sex)
{
  _sex = sex;
  set_modified();
}

void Creature::setAttribute(Attribute atr, int val)
{
  _stats.setAttribute(atr,val);
  set_modified();
}

void Creature::modifyAttribute(Attribute atr, int mod)
{
  _stats.modifyAttribute(atr, mod);
  set_modified();
}

void Creature::setSkill(Skill skill, int val)
{
  _stats.setSkill(skill, val);
  set_modified();
}

void Creature::modifySkill(Skill skill, int mod)
{
  _stats.modifySkill(skill, mod);
  set_modified();
}

void Creature::take(ItemPtr item, int amount)
{  
  inventoryContainer()->insert(item, amount);
  set_modified();
}

AmountedItem Creature::drop(dbRef item_ref, int amount)
{
  set_modified();
  return inventoryContainer()->erase(item_ref, amount);
}

std::vector<AmountedItem > Creature::inventory()
{
  return inventoryContainer()->getAll();
}

void Creature::equip(ItemPtr item)
{
  _body.equip(item);

  if (!item->mods()->getAll().empty())
  {
    _mods->add( item->mods()->get_complex_mod() );
  }

  calcWeapons();
  set_modified();
}

ItemPtr Creature::unequip(dbRef item_ref)
{
  ItemPtr r = _body.unequip(item_ref);

  //usun modyfikatory
  if (r != nullptr)
    {
      _mods->remove(r->mods()->get_complex_mod()->ref());
    }

  calcWeapons();
  set_modified();

  return r;
}

Item *Creature::weapon()
{
  return _weapon;
}

Item *Creature::offhand()
{
  return _offhand;
}

Item *Creature::shield()
{
  return _shield;
}

void Creature::setLocation(Location *loc)
{
  _prevLoc = _currentLoc;
  _currentLoc = loc;
}

void Creature::setInventory(Item::Container* newInventory)
{
  _inventory.reset(newInventory);
}

void Creature::calcTotalDamage()
{
  //TODO
  //algorytm z systemu rpg
  //na podstawie damage_level z bodyParts
}
