#include "item.h"

using namespace std;

const dbTable Item::table_name = "items";
PrototypeManager<Item, ItemPrototypes>* Item::_prototypes = nullptr;

Item::Item(dbRef ref)
: DBObject(ref)
{
}

std::unique_ptr<Item> Item::create(dbRef ref)
{
  //TODO
  return unique_ptr<Item>(nullptr);
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

