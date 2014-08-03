#include "item.h"
#include "ordinaryitem.h"
#include "weapon.h"
#include "armor.h"
#include "jewelry.h"
#include "food.h"
#include "tool.h"
#include "shield.h"
#include "../World/locationobject.h"
#include "item_container.h"

using namespace std;
using namespace soci;
using namespace fun;

//==========ITEM============
const dbTable Item::tableName = "items";

Item::Item(dbRef ref, bool temporary)
  : DBObject(ref, temporary)
  , _item_type(ItemType::Null)
  , _name("")
  , _descript("")
  , _weight(0)
  , _value(0)
  , _condition(ItemCondition::Null)
  , _durability(0)
  , _stackable(false)
  , _mods(this)
{  
}

Item *Item::create(dbRef ref, bool prototype, bool temporary)
{
  Item* new_item = nullptr;

  if (ref > 0)
  {
    MapRow item_data = MapQuery("SELECT obj_type, item_type FROM "+tableName+" WHERE ref="+toStr(ref));
    ItemType item_type = CheckFieldCast<ItemType>( item_data["ITEM_TYPE"] );
    ObjType obj_type = CheckFieldCast<ObjType>( item_data["OBJ_TYPE"] );

    if (item_type != ItemType::Null && (obj_type == ObjType::Instance || prototype) )
    {
      switch(item_type)
      {
        case ItemType::Ordinary: new_item = new OrdinaryItem(ref, temporary); break;
        case ItemType::Weapon: new_item = new Weapon(ref, temporary); break;
        case ItemType::Armor: new_item = new Armor(ref, temporary); break;
        case ItemType::Jewelry: new_item = new Jewelry(ref, temporary); break;
        case ItemType::Food: new_item = new Food(ref, temporary); break;
        case ItemType::Tool: new_item = new Tool(ref, temporary); break;
        case ItemType::Shield: new_item = new Shield(ref, temporary); break;
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
  if ( !loaded() && ref() > 0 ){
    try
    {      
      MapRow item_data;
      if (data_source != nullptr)
      {
        item_data = *data_source;
      }
      else
      {
        item_data = MapQuery( "SELECT * FROM "+table()+" WHERE ref="+toStr(ref()) );
      }

      if (!item_data.empty())
      {
        setType( CheckFieldCast<ItemType>(item_data["ITEM_TYPE"]));
        setName( CheckField<string>(item_data["NAME"]) );
        setDescript( CheckField<string>(item_data["DESCRIPTION"]) );
        setWeight( CheckField<double>(item_data["WEIGHT"]) );
        setValue( CheckField<int>(item_data["SHOP_VALUE"]) );
        setCondition( CheckFieldCast<ItemCondition>(item_data["CONDITION"]));
        _bodyParts.clear();
        _bodyParts = Str2BodyParts( CheckField<string>(item_data["BODY_PARTS"]) );
        setDurability( CheckField<int>(item_data["DURABILITY"]) );
        setStackable( CheckField<bool>(item_data["STACKABLE"]) );

        _mods.get_complex_mod()->setName( name() );

        set_loaded();
        set_not_modified();
      }

      //MODS
      //zaladuj z crt_mods
      vector<int> mod_refs(100);
      vector<indicator> inds;

      string query = "SELECT ref FROM crt_mods WHERE otable='" + table() + "' and oref=" + fun::toStr(ref());
      _Database << query, into(mod_refs, inds);

      for (auto m = mod_refs.begin(); m != mod_refs.end(); ++m)
        _mods.add( shared_ptr<CreatureModificator>(new CreatureModificator(*m)) );

      //INVENTORY
      dbRef inv_ref = Item::Container::byOwner(table(), ref());
      if (inv_ref != 0)
      {
        _inventory.reset(new Item::Container(inv_ref));
      }
      else
      {
        _inventory.reset();
      }

    }
    catch(soci_error &e)
    {
      MsgError(e.what());
      qDebug() << _Database.get_last_query().c_str();
    }
  }
}

void Item::saveToDB()
{

  stringstream save_query;

  save_query <<
    "UPDATE " << table() << " SET " <<
    "  ITEM_TYPE = " << static_cast<int>(_item_type) <<
    ", NAME = '" << _name << "'"
    ", DESCRIPTION = '" << _descript << "'"
    ", WEIGHT = " << _weight <<
    ", SHOP_VALUE = " << _value <<
    ", CONDITION = " << static_cast<int>(_condition) <<
    ", DURABILITY = " << _durability <<
    ", BODY_PARTS = '" << BodyParts2Str(_bodyParts) << "'"
    ", STACKABLE = " << static_cast<int>(_stackable) <<
    " WHERE ref = " << ref();

  save(save_query.str());  
  DBObject::saveToDB();
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

Item::~Item()
{
  _saveToDB_
}
//===~~~

