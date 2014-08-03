#include "item_container.h"

using namespace std;
using namespace soci;

const dbTable Item::Container::tableName = "item_containers";

Item::Container::Container(dbRef ref, bool temporary)
  : DBObject(ref, temporary)
{
  load();
}

Item::Container::Container(int pojemnosc)
  : DBObject(0)
{
  dbRef new_ref;
  _Database << "select new_ref from create_new_item_container", into(new_ref);

  set_ref(new_ref);
  set_max_weight(pojemnosc);

  set_loaded();
  set_modified();
}

Item::Container::~Container()
{
  _saveToDB_
}

string Item::Container::items2str()
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

void Item::Container::str2items(string items)
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
          insert(item, amount);
        }
      }
      else
      {
        qDebug() << "Problem z wczytaniem itema z item containera " << ref() << " : " << (*ai).c_str();
      }
    }
  }

}

dbRef Item::Container::byOwner(dbTable otable, dbRef oref)
{
  dbRef ref = 0;
  soci::indicator ind;
  _Database << "SELECT ref FROM item_containers WHERE otable='"<<otable<<"' and oref="<<fun::toStr(oref), into(ref, ind);
  if (ind != soci::i_ok) ref = 0;
  return ref;
}

void Item::Container::load(MapRow *data_source)
{
  if ( !loaded() && ref() > 0 ){
    try
    {
      MapRow cont_data;
      if (data_source != nullptr)
      {
        cont_data = *data_source;
      }
      else
      {
        cont_data = fun::MapQuery("SELECT name, otable, oref, max_weight, items FROM "+tableName+" WHERE ref="+fun::toStr(ref()));
      }

      if (!cont_data.empty())
      {
        setName( fun::CheckField<std::string>(cont_data["NAME"]) );
        set_max_weight( fun::CheckField<Weight>(cont_data["MAX_WEIGHT"]) );
        setORef( fun::CheckField<dbRef>(cont_data["OREF"]) );
        setOTable( fun::CheckField<std::string>(cont_data["OTABLE"]) );
        str2items( fun::CheckField<string>(cont_data["ITEMS"]));
      }

      set_loaded();
      set_not_modified();
    }
    catch(soci::soci_error &e)
    {
      fun::MsgError(e.what());
      qDebug() << _Database.get_last_query().c_str();
    }
    catch(std::exception &e)
    {
      fun::MsgError(e.what());
    }
  }
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

void Item::Container::saveToDB()
{
  std::stringstream save_query;

  save_query << "UPDATE " << table() << " SET"
             << "  MAX_WEIGHT=" << _weight_cap.max
             << " ,NAME='" << _name << "'"
             << " ,OREF=" << _oref
             << " ,OTABLE='" << _otable << "'"
             << " ,ITEMS='" << items2str() << "'"
             << " WHERE ref=" << ref();

  save(save_query.str());
  DBObject::saveToDB();
}

//===~~~

