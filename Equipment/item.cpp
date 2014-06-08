#include "item.h"
#include "ordinaryitem.h"
#include "weapon.h"
#include "armor.h"
#include "jewelry.h"
#include "food.h"
#include "tool.h"

using namespace std;
using namespace soci;
using namespace fun;

const dbTable Item::table_name = "items";

Item::Item(dbRef ref) : DBObject(ref)
{
}

std::unique_ptr<Item> Item::create(dbRef ref, bool prototype)
{
  Item* new_item = nullptr;

  MapRow item_data = MapQuery("SELECT obj_type, item_type FROM "+table_name+" WHERE ref="+toStr(ref));
  ItemType item_type = CheckFieldCast<ItemType>( item_data["ITEM_TYPE"] );
  ObjType obj_type = CheckFieldCast<ObjType>( item_data["OBJ_TYPE"] );

  if (item_type != ItemType::Null && (obj_type == ObjType::Instance || prototype) )
  {
    switch(item_type)
    {
      case ItemType::Ordinary: new_item = new OrdinaryItem(ref); break;
      case ItemType::Weapon: new_item = new Weapon(ref); break;
      case ItemType::Armor: new_item = new Armor(ref); break;
      case ItemType::Jewelry: new_item = new Jewelry(ref); break;
      case ItemType::Food: new_item = new Food(ref); break;
      case ItemType::Tool: new_item = new Tool(ref); break;
      default : throw creation_error("Nieprawidłowy typ itemu."); break;
    }
  }else throw creation_error("Brak prawidłowego rekordu w bazie.");

  new_item->load();

  return unique_ptr<Item>(new_item);
}

void Item::load()
{
  if ( !loaded() && ref() > 0 ){
    try
    {
      MapRow item_data = MapQuery( "SELECT * FROM "+table()+" WHERE ref="+toStr(ref()) );
      if (item_data.size() > 0)
      {
        set_item_type( CheckFieldCast<ItemType>(item_data["ITEM_TYPE"]));
        set_name( CheckField<string>(item_data["NAME"]) );
        set_destript( CheckField<string>(item_data["DESCRIPTION"]) );
        set_weight( CheckField<double>(item_data["WEIGHT"]) );
        set_value( CheckField<int>(item_data["SHOP_VALUE"]) );

        set_loaded();
      }
    }
    catch(soci_error &e)
    {
      MsgError(e.what());
      qDebug() << _Database.get_last_query().c_str();
    }
  }
}

std::unique_ptr<Item> Item::clone()
{
  //save
  save_to_db();

  //clone db record
  dbRef new_ref(0);
  _Database << "EXECUTE PROCEDURE CLONE_ITEM("<< ref() << ")", into(new_ref);
  _Database.commit();

  //return new item
  return Item::create(new_ref);
}

void Item::set_item_type(ItemType type)
{
  _item_type = type;
  save("ITEM_TYPE",static_cast<int>(_item_type));
}

void Item::set_size_class(ItemSizeClass size_class)
{
  _size_class = size_class;
  save("SIZE_CLASS",static_cast<int>(_size_class));
}

void Item::set_body_part(BodyPart body_part)
{
  _body_part = body_part;
  save("BODY_PART",static_cast<int>(_body_part));
}

void Item::set_name(string name)
{
  _name = name;
  save("NAME", _name);
}

void Item::set_destript(string dsc)
{
  _descript = dsc;
  save("DESCRIPTION", _descript);
}

void Item::set_weight(double weight)
{
  _weight = weight;
  save("WEIGHT", _weight);
}

void Item::set_value(int value)
{
  _value = value;
  save("SHOP_VALUE", _value);
}

Item::~Item()
{
}

