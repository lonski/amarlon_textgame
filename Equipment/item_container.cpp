#include "item_container.h"
#include "Include/functions/messages.h"
#include "Include/data_gateways/db_gateways/item_containers_gateway_db.h"

using namespace std;
using namespace soci;

const dbTable Item::Container::tableName = "item_containers";
DataGateway* Item::Container::containersGateway = new ItemContainersGatewayDB;

Item::Container::Container(dbRef ref)
  : DBObject(ref)
{
}

/* if ref does not exists in DataSource this function will return
 * a new Item::Container with bland record inserted to DB.
 */
Item::Container *Item::Container::create(dbRef ref)
{
  Item::Container* newContainer = dynamic_cast<Item::Container*>(containersGateway->fetch(ref));

  if (newContainer->ref() == 0)
  {
    containersGateway->write(newContainer);
  }

  return newContainer;
}

Item::Container::~Container()
{
  if (ref() && modified())
    saveToDB();
}

void Item::Container::load(MapRow *data_source)
{
  if ( !loaded() && ref() > 0 )
    containersGateway->fetchInto(this, ref());
}

void Item::Container::saveToDB()
{
  containersGateway->write(this);
}

dbRef Item::Container::byOwner(dbTable otable, dbRef oref)
{
  dbRef ref = 0;
  soci::indicator ind;
  _Database << "SELECT ref FROM item_containers WHERE otable='"<<otable<<"' and oref="<<fun::toStr(oref), into(ref, ind);
  if (ind != soci::i_ok) ref = 0;
  return ref;
}

string Item::Container::getItemsString()
{
  string str;

  for (auto i = _items.begin(); i != _items.end(); ++i)
  {
    string item = fun::toStr(i->first);
    string amount = fun::toStr(i->second.amount);
    str += (item + "," + amount + ";");
  }

  return str;
}

void Item::Container::setItems(string items)
{
  _items.clear();
  if (!items.empty())
  {
    vector<string> amounted_items = fun::explode(items,';');

    for (auto ai = amounted_items.begin(); ai != amounted_items.end(); ++ai)
    {
      vector<string> one_ai = fun::explode(*ai, ',');
      if (one_ai.size() == 2)
      {
        dbRef item_ref = fun::fromStr<dbRef>(one_ai[0]);
        int amount = fun::fromStr<int>(one_ai[1]);

        if (item_ref > 0 && amount > 0)
        {
          ItemPtr item( Item::create(item_ref) );
          if (item != nullptr)
          {
            insert(item, amount);
          }
          else
          {
            qDebug() << "Non valid item ref in invetory: " << item_ref;
          }
        }
      }
      else
      {
        qDebug() << "Problem z wczytaniem itema z item containera " << ref() << " : " << (*ai).c_str();
      }
    }
  }

}

/*
 *  Passing a reference (not by value), because otherwise if insert fail, the item could be lost.
 */
void Item::Container::insert(ItemPtr &item, int amount)
{
  if ( !item->isStackable() ) amount = 1;
  Weight items_weight = amount * item->weight();

  if ( _weight_cap.cur + items_weight > _weight_cap.max )
  {
    qDebug() << "Cur cap [" << _weight_cap.cur << "] + items_weight [" << items_weight << "] < max_cap [" << _weight_cap.max << "]";
    throw error::container_insertion_error("Zbyt duży ciężar, aby pomieścić go w pojemniku.");
  }
  else
  {
    if (item->isStackable() && _items.find(item->ref()) != _items.end() )
    {
      AmountedItem& aitem = _items[item->ref()];
      aitem.amount += amount;
    }
    else
    {
      _items.insert(std::make_pair<dbRef, AmountedItem >(item->ref(), AmountedItem(item, amount)) );
    }

    set_modified();
  }
}

AmountedItem Item::Container::erase(dbRef item_ref, int amount)
{
  auto iter = _items.find(item_ref);
  AmountedItem ret;
  if (iter != _items.end())
  {
    ret = iter->second;

    bool stackable = iter->second.item->isStackable();
    int& cont_amount = iter->second.amount;

    if (!stackable) amount = 1;

    if (!stackable || cont_amount <= amount )
    {
      _items.erase(item_ref);
    }
    else
    {
      cont_amount -= amount;
      ret.amount = amount;
    }

    set_modified();
  }

  return ret;
}

AmountedItem Item::Container::find(dbRef item_ref)
{
  AmountedItem ret;
  auto iter = _items.find(item_ref);
  if (iter != _items.end()) ret = iter->second;

  return ret;
}

std::vector<AmountedItem > Item::Container::getAll()
{
  std::vector<AmountedItem > ret;
  ret.reserve(_items.size());

  for (auto iter = _items.begin(); iter != _items.end(); ++iter)
    ret.push_back(iter->second);

  return ret;
}

AmountedItem Item::Container::get(size_t index)
{
  AmountedItem r;
  if (index < _items.size())
  {
    vector<AmountedItem> ai = getAll();
    r = ai[index];
  }

  return r;
}

size_t Item::Container::count()
{
  return _items.size();
}

void Item::Container::setName(std::string name)
{
  _name = name;
  set_modified();
}

void Item::Container::setOTable(dbTable otable)
{
  _otable = otable;
  set_modified();
}

void Item::Container::setORef(dbRef oref)
{
  _oref = oref;
  set_modified();
}

void Item::Container::set_max_weight(Weight max_weight)
{
  _weight_cap.max = max_weight;
  set_modified();
}


