#include "item.h"

using namespace std;

PrototypeManager<Item, ItemPrototypes> Item::Prototypes;

Item::Item(dbRef ref)
: DBObject(ref, "items")
{
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

