#include "item.h"
#include "../World/locationobject.h"
#include "item_container.h"
#include "Include/functions/messages.h"
#include "Include/data_gateways/db_gateways/items_gateway_db.h"

using namespace std;
using namespace soci;
using namespace fun;

//==========ITEM============
const dbTable Item::tableName = "items";
DataGateway* Item::itemsGateway = new ItemsGatewayDB;

Item::Item(dbRef ref, bool temporary)
  : DBObject(ref, temporary)
  , _mods(this)
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

Item *Item::create(dbRef ref, bool prototype, bool temporary)
{
  Item* new_item = nullptr;

  if (ref > 0)
  {
    MapRow item_data = MapQuery("SELECT obj_type, item_type FROM "+tableName+" WHERE ref="+toStr(ref));
    ItemType item_type = CheckValueCast<ItemType>( item_data["ITEM_TYPE"] );
    ObjType obj_type = CheckValueCast<ObjType>( item_data["OBJ_TYPE"] );

    if (item_type != ItemType::Null && (obj_type == ObjType::Instance || prototype) )
    {
      switch(item_type)
      {
        case ItemType::Ordinary: new_item = new Item(ref, temporary); break;
        case ItemType::Weapon: new_item = new Item(ref, temporary); break;
        case ItemType::Armor: new_item = new Item(ref, temporary); break;
        case ItemType::Jewelry: new_item = new Item(ref, temporary); break;
        case ItemType::Food: new_item = new Item(ref, temporary); break;
        case ItemType::Tool: new_item = new Item(ref, temporary); break;
        case ItemType::Shield: new_item = new Item(ref, temporary); break;
        case ItemType::LocationObject: new_item = new LocationObject(ref, temporary); break;
        default : throw error::creation_error("Nieprawidłowy typ itemu."); break;
      }
    }else throw error::creation_error("Brak prawidłowego rekordu w bazie.");

    new_item->load();
  }

  return new_item;
}

void Item::load(MapRow *data_source)
{
  if ( !loaded() && ref() > 0 )
    itemsGateway->fetchInto(this);
//  if ( !loaded() && ref() > 0 ){
//    try
//    {
//      MapRow item_data;
//      if (data_source != nullptr)
//      {
//        item_data = *data_source;
//      }
//      else
//      {
//        item_data = MapQuery( "SELECT * FROM "+table()+" WHERE ref="+toStr(ref()) );
//      }

//      if (!item_data.empty())
//      {
//        //item
//        setType( CheckValueCast<ItemType>(item_data["ITEM_TYPE"]));
//        setName( CheckValue<string>(item_data["NAME"]) );
//        setDescript( CheckValue<string>(item_data["DESCRIPTION"]) );
//        setWeight( CheckValue<double>(item_data["WEIGHT"]) );
//        setValue( CheckValue<int>(item_data["SHOP_VALUE"]) );
//        setCondition( CheckValueCast<ItemCondition>(item_data["CONDITION"]));
//        setBodyParts( CheckValue<string>(item_data["BODY_PARTS"]) );
//        setDurability( CheckValue<int>(item_data["DURABILITY"]) );
//        setStackable( CheckValue<bool>(item_data["STACKABLE"]) );

//        //weapon
//        setWeaponSkill(CheckValueCast<WeaponSkill>(item_data["WPN_SKILL"]));
//        setDefence(CheckValue<int>(item_data["WPN_DEFENCE"]));
//        setAttack(CheckValue<int>(item_data["WPN_ATTACK"]));
//        setReflex(CheckValue<int>(item_data["WPN_REFLEX"]));
//        setStrReq(CheckValue<int>(item_data["WPN_STR_REQ"]));
//        setRange(CheckValue<int>(item_data["WPN_RANGE"]));

//        Damage dmg
//        (
//          CheckValue<int>(item_data["WPN_D_PIERCING"]),
//          CheckValue<int>(item_data["WPN_D_SLASHING"]),
//          CheckValue<int>(item_data["WPN_D_BASHING"])
//        );

//        setDamage(dmg);

//        //armor
//        Damage dmgred
//        (
//          CheckValue<int>(item_data["ARM_DR_PIERCING"]),
//          CheckValue<int>(item_data["ARM_DR_SLASHING"]),
//          CheckValue<int>(item_data["ARM_DR_BASHING"])
//        );

//        setDamageReduction(dmgred);

//        //food
//        setHunger(CheckValue<int>(item_data["FOD_HUNGER"]));

//        //

//        _mods.get_complex_mod()->setName( name() );

//        set_loaded();
//        set_not_modified();
//      }

//      //MODS
//      //zaladuj z crt_mods
//      vector<int> mod_refs(100);
//      vector<indicator> inds;

//      string query = "SELECT ref FROM crt_mods WHERE otable='" + table() + "' and oref=" + fun::toStr(ref());
//      _Database << query, into(mod_refs, inds);

//      for (auto m = mod_refs.begin(); m != mod_refs.end(); ++m)
//        _mods.add( shared_ptr<CreatureModificator>(new CreatureModificator(*m)) );

//      //INVENTORY
//      dbRef inv_ref = Item::Container::byOwner(table(), ref());
//      if (inv_ref != 0)
//      {
//        _inventory.reset(new Item::Container(inv_ref));
//      }
//      else
//      {
//        _inventory.reset();
//      }

//    }
//    catch(soci_error &e)
//    {
//      MsgError(e.what());
//      qDebug() << _Database.get_last_query().c_str();
//    }
//  }
}

void Item::saveToDB()
{
  if (!isTemporary())
    itemsGateway->write(this);
//  stringstream save_query;

//  save_query <<
//    "UPDATE " << table() << " SET " <<
//    "  ITEM_TYPE = " << static_cast<int>(_item_type) <<
//    ", NAME = '" << _name << "'"
//    ", DESCRIPTION = '" << _descript << "'"
//    ", WEIGHT = " << _weight <<
//    ", SHOP_VALUE = " << _value <<
//    ", CONDITION = " << static_cast<int>(_condition) <<
//    ", DURABILITY = " << _durability <<
//    ", BODY_PARTS = '" << getBodyPartsString() << "'"
//    ", STACKABLE = " << static_cast<int>(_stackable)
//             << " ,WPN_SKILL=" << static_cast<int>(_wpn_skill)
//             << " ,WPN_D_PIERCING=" << _damage.piercing
//             << " ,WPN_D_SLASHING=" << _damage.slashing
//             << " ,WPN_D_BASHING=" << _damage.bashing
//             << " ,WPN_DEFENCE=" << _defence
//             << " ,WPN_ATTACK=" << _attack
//             << " ,WPN_REFLEX=" << _reflex
//             << " ,WPN_STR_REQ=" << _str_req
//             << " ,WPN_RANGE=" << _range
//             << " ,ARM_DR_PIERCING=" << _damage_red.piercing
//             << " ,ARM_DR_SLASHING=" << _damage_red.slashing
//             << " ,ARM_DR_BASHING=" << _damage_red.bashing
//             << " ,FOD_HUNGER=" << _hunger
//             <<
//    " WHERE ref = " << ref();

//  save(save_query.str());
//  DBObject::saveToDB();
}

Item::Inventory &Item::inventory()
{
  if (_inventory == nullptr)
  {
    //TODO REFACTOR

    _inventory.reset( new Item::Container );
    _inventory->setORef(ref());
    _inventory->setOTable(table());
  }

  return _inventory;
}

void Item::setInventory(Item::Container *inv)
{
  _inventory.reset(inv);
}

CreatureModificatorManager &Item::mods()
{
  return _mods;
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

std::vector<BodyPartType> Item::bodyParts() const
{
  return _bodyParts;
}

bool Item::checkBodyPart(BodyPartType bp) const
{
  return std::find(_bodyParts.begin(), _bodyParts.end(), bp) != _bodyParts.end();
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

Item *Item::clone()
{
  if (!isTemporary())
  {
    //save
    saveToDB();

    //clone db record
    dbRef new_ref(0);
    _Database << "EXECUTE PROCEDURE CLONE_ITEM("<< ref() << ")", into(new_ref);
    _Database.commit();

    //return new item
    return Item::create(new_ref);
  }else throw error::creation_error("Nie można sklonować obiektu tymczasowego!");

  return nullptr;
}

Item *Item::forge(ItemPrototype proto)
{
  return Item::prototypes().clone(proto);
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

void Item::addBodyPart(BodyPartType body_part)
{
  if (checkBodyPart(body_part) == false )
  {
    _bodyParts.push_back(body_part);
    set_modified();
  }
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

Item::~Item()
{
  _saveToDB_
}
//===~~~

