#include "item.h"

using namespace std;
using namespace fun;

const dbTable Item::table_name = "items";
PrototypeManager<Item, ItemPrototypes>* Item::_prototypes = nullptr;

Item::Item(dbRef ref) : DBObject(ref)
{
}

std::unique_ptr<Item> Item::create(dbRef ref)
{
  Item* new_item = nullptr;

  MapRow item_data = MapQuery("SELECT obj_type, item_type FROM "+table_name+" WHERE ref="+toStr(ref));
  ItemTypes item_type = CheckFieldCast<ItemTypes>( item_data["ITEM_TYPE"] );
  ObjTypes obj_type = CheckFieldCast<ObjTypes>( item_data["OBJ_TYPE"] );

  if (item_type != ItemTypes::Null && obj_type == ObjTypes::Instance)
  {
    switch(item_type)
    {
      case ItemTypes::Ordinary: new_item = new OrdinaryItem(ref); break;
      case ItemTypes::Weapon: new_item = new Weapon(ref); break;
      case ItemTypes::Armor: new_item = new Armor(ref); break;
      case ItemTypes::Jewelry: new_item = new Jewelry(ref); break;
      case ItemTypes::Food: new_item = new Food(ref); break;
      case ItemTypes::Tool: new_item = new Tool(ref); break;
      default : throw creation_error("Nieprawidłowy typ itemu."); break;
    }
  }else throw creation_error("Brak prawidłowego rekordu w bazie.");

  new_item->load();

  return unique_ptr<Item>(new_item);
}

PrototypeManager<Item, ItemPrototypes> &Item::prototypes()
{
  if (_prototypes == nullptr) _prototypes = new PrototypeManager<Item, ItemPrototypes>();
  return *_prototypes;
}

void Item::load()
{
  //TODO
}

std::unique_ptr<Item> Item::clone()
{
  //TODO
  return unique_ptr<Item>(nullptr);
}

Item::~Item()
{
}

