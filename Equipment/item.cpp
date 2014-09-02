#include "item.h"
#include "item_container.h"
#include "Include/functions/messages.h"
#include "Include/data_gateways/db_gateways/items_gateway_db.h"

using namespace std;
using namespace soci;
using namespace fun;

const dbTable Item::tableName = "items";
DataGateway* Item::itemsGateway = new ItemsGatewayDB;

Item::Item(dbRef ref)
  : DBObject(ref)
  , _mods(new CreatureModificatorManager(this))
  , _item_type(ItemType::Null)
  , _name("")
  , _descript("")
  , _weight(0)
  , _value(0)
  , _condition(ItemCondition::Null)
  , _durability(0)
  , _stackable(false)
  , _wpn_skill(WeaponSkill::Null)
  , _defence(0)
  , _attack(0)
  , _reflex(0)
  , _str_req(0)
  , _range(0)
  , _hunger(0)
{  
}

Item::~Item()
{
  if (ref() && modified())
    saveToDB();

  delete _mods;
  _mods = nullptr;
}

Item *Item::create(dbRef ref, bool prototype)
{
  //todo obsluga prototye?
  return dynamic_cast<Item*>(itemsGateway->fetch(ref));
}

Item *Item::forge(ItemPrototype proto)
{
  return Item::prototypes().clone(proto);
}

Item *Item::clone()
{
  return dynamic_cast<Item*>(itemsGateway->clone(this));
}

void Item::load(MapRow*)
{
  if ( !loaded() && ref() > 0 )
    itemsGateway->fetchInto(this, ref());
}

void Item::saveToDB()
{
  itemsGateway->write(this);
}

bool Item::checkBodyPart(BodyPartType bp) const
{
  return std::find(_bodyParts.begin(), _bodyParts.end(), bp) != _bodyParts.end();
}

void Item::addBodyPart(BodyPartType body_part)
{
  _bodyParts.push_back(body_part);
  set_modified();
}

void Item::removeBodyPart(BodyPartType body_part)
{
  auto iter = std::find(_bodyParts.begin(), _bodyParts.end(), body_part);
  if (iter != _bodyParts.end())
  {
    _bodyParts.erase(iter);
    set_modified();
  }
}

std::vector<BodyPartType> Item::bodyParts() const
{
  return _bodyParts;
}

Item::Inventory &Item::inventory()
{
  if (_inventory == nullptr)
  {
    //TODO REFACTOR

    _inventory.reset(  Item::Container::create() );
    _inventory->setORef(ref());
    _inventory->setOTable(table());
  }

  return _inventory;
}

void Item::setInventory(Item::Container *inv)
{
  _inventory.reset(inv);
}

CreatureModificatorManager *Item::mods()
{
  return _mods;
}

void Item::setCreatureModificatorManager(CreatureModificatorManager *mods)
{
  _mods = mods;
}

void Item::setType(ItemType type)
{
  _item_type = type;
  set_modified();
}

void Item::setName(string name)
{
  _name = name;
  set_modified();
}

void Item::setDescript(string dsc)
{
  _descript = dsc;
  set_modified();
}

void Item::setWeight(Weight weight)
{
  _weight = weight;
  set_modified();
}

void Item::setValue(int value)
{
  _value = value;
  set_modified();
}

void Item::setCondition(ItemCondition condition)
{
  _condition = condition;
  set_modified();
}

void Item::setDurability(int dura)
{
  _durability = dura;
  set_modified();
}

void Item::setStackable(bool stackable)
{
  _stackable = stackable;
  set_modified();
}

std::vector<BodyPartType> Item::setBodyParts(const string &str)
{
  vector<BodyPartType> p;

  vector<string> sv = fun::explode(str, ',');
  for (auto s = sv.begin(); s != sv.end(); ++s)
    p.push_back(static_cast<BodyPartType>( fun::fromStr<int>(*s) ));

  _bodyParts.clear();
  _bodyParts = p;

  return p;
}

/*
 * Function converts BodyPartTypes to a 0-1 string, which is saved to db
 */
string Item::getBodyPartsString()
{
  string str("");

  for (auto i = _bodyParts.begin(); i != _bodyParts.end(); ++i)
  {
    BodyPartType bp = *i;
    str += fun::toStr(static_cast<int>(bp)) + ",";
  }

  return str;
}


ItemType Item::type() const
{
  return _item_type;
}

string Item::name() const
{
  return _name;
}

string Item::descript() const
{
  return _descript;
}

Weight Item::weight() const
{
  return _weight;
}

int Item::value() const
{
  return _value;
}

ItemCondition Item::condition() const
{
  return _condition;
}

int Item::durability() const
{
  return _durability;
}

bool Item::isStackable() const
{
  return _stackable;
}

void Item::setWeaponSkill(WeaponSkill skill)
{
  _wpn_skill = skill;
  set_modified();
}

void Item::setDamage(Damage damage)
{
  _damage = damage;
  set_modified();
}

void Item::setDamageReduction(Damage dmg_red)
{
  _damage_red = dmg_red;
  set_modified();
}

void Item::setDefence(int defence)
{
  _defence = defence;
  set_modified();
}

void Item::setAttack(int attack)
{
  _attack = attack;
  set_modified();
}

void Item::setReflex(int reflex)
{
  _reflex = reflex;
  set_modified();
}

void Item::setStrReq(int val)
{
  _str_req = val;
  set_modified();
}

void Item::setRange(int range)
{
  _range = range;
  set_modified();
}

void Item::setHunger(int hunger)
{
  _hunger = hunger;
  set_modified();
}
