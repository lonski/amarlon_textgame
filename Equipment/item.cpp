#include "item.h"
#include "ordinaryitem.h"
#include "weapon.h"
#include "armor.h"
#include "jewelry.h"
#include "food.h"
#include "tool.h"
#include "shield.h"
#include "../World/locationobject.h"

using namespace std;
using namespace soci;
using namespace fun;

//==========ITEM============
const dbTable Item::table_name = "items";

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

std::unique_ptr<Item> Item::create(dbRef ref, bool prototype, bool temporary)
{
  Item* new_item = nullptr;

  if (ref > 0)
  {
    MapRow item_data = MapQuery("SELECT obj_type, item_type FROM "+table_name+" WHERE ref="+toStr(ref));
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

  return unique_ptr<Item>(new_item);
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
        set_type( CheckFieldCast<ItemType>(item_data["ITEM_TYPE"]));
        set_name( CheckField<string>(item_data["NAME"]) );
        set_descript( CheckField<string>(item_data["DESCRIPTION"]) );
        set_weight( CheckField<double>(item_data["WEIGHT"]) );
        set_value( CheckField<int>(item_data["SHOP_VALUE"]) );
        set_condition( CheckFieldCast<ItemCondition>(item_data["CONDITION"]));
        _body_parts.clear();
        _body_parts = Str2BodyParts( CheckField<string>(item_data["BODY_PARTS"]) );        
        set_durability( CheckField<int>(item_data["DURABILITY"]) );
        set_stackable( CheckField<bool>(item_data["STACKABLE"]) );

        _mods.get_complex_mod()->set_name( name() );

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
      try
      {
        _inventory = Container<>::create(Container<>::byOwner( table(),ref() ));
      }
      catch(error::creation_error)
      {
        _inventory = unique_ptr<Container<> >(nullptr);
      }
    }
    catch(soci_error &e)
    {
      MsgError(e.what());
      qDebug() << _Database.get_last_query().c_str();
    }
  }
}

void Item::save_to_db()
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
    ", BODY_PARTS = '" << BodyParts2Str(_body_parts) << "'"
    ", STACKABLE = " << static_cast<int>(_stackable) <<
    " WHERE ref = " << ref();

  save(save_query.str());  
  DBObject::save_to_db();
}

std::unique_ptr<Item> Item::clone()
{
  if (!isTemporary())
  {
    //save
    save_to_db();

    //clone db record
    dbRef new_ref(0);
    _Database << "EXECUTE PROCEDURE CLONE_ITEM("<< ref() << ")", into(new_ref);
    _Database.commit();

    //return new item
    return Item::create(new_ref);
  }else throw error::creation_error("Nie można sklonować obiektu tymczasowego!");

  return unique_ptr<Item>(nullptr);
}

void Item::set_type(ItemType type)
{
  _item_type = type;
  set_modified();
}

void Item::set_name(string name)
{
  _name = name;
  set_modified();
}

void Item::set_descript(string dsc)
{
  _descript = dsc;
  set_modified();
}

void Item::set_weight(Weight weight)
{
  _weight = weight;
  set_modified();
}

void Item::set_value(int value)
{
  _value = value;
  set_modified();
}

void Item::set_condition(ItemCondition condition)
{
  _condition = condition;
  set_modified();
}

void Item::set_durability(int dura)
{
  _durability = dura;
  set_modified();
}

void Item::add_body_part(BodyPartType body_part)
{
  if (check_body_part(body_part) == false )
  {
    _body_parts.push_back(body_part);
    set_modified();
  }
}

void Item::remove_body_part(BodyPartType body_part)
{
  auto iter = std::find(_body_parts.begin(), _body_parts.end(), body_part);
  if (iter != _body_parts.end())
  {
    _body_parts.erase(iter);
    set_modified();
  }
}

void Item::set_stackable(bool stackable)
{
  _stackable = stackable;
  set_modified();
}

Item::~Item()
{
  _SAVE_TO_DB_
}
//===~~~


