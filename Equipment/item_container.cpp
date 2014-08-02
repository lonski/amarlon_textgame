#include "item.h"

const dbTable Item::Container::table_name = "item_containers";

Item::Container::Container(dbRef ref, bool temporary)
  : DBObject(ref, temporary)
{
  load();
}

Item::Container::Container()
  : DBObject(0)
{
  set_ref(-1);
  //TODO
}

Item::Container::~Container()
{
  _saveToDB_
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
        cont_data = fun::MapQuery("SELECT name, otable, oref, max_weight, items FROM "+table_name+" WHERE ref="+fun::toStr(ref()));
      }

      if (!cont_data.empty())
      {
        set_name( fun::CheckField<std::string>(cont_data["NAME"]) );
        set_max_weight( fun::CheckField<Weight>(cont_data["MAX_WEIGHT"]) );
        set_oref( fun::CheckField<dbRef>(cont_data["OREF"]) );
        set_otable( fun::CheckField<std::string>(cont_data["OTABLE"]) );
      }

      //==pos data LOAD Z POLA ITEMS
//      _items.clear();
//      MapTable pos_data;
//      fun::MapQuery("SELECT * FROM item_container_pos WHERE container="+fun::toStr(ref()), pos_data);
//      if (pos_data.size() > 0)
//      {
//        for (auto RowIter = pos_data.begin(); RowIter != pos_data.end(); ++RowIter)
//        {
//          dbRef item_ref = fun::CheckField<dbRef>( (*RowIter)["ITEM"] );
//          int amount = fun::CheckField<int>( (*RowIter)["AMOUNT"] );

//          std::shared_ptr<Item> item( dynamic_cast<T*>(Item::create(item_ref).release()) );
//          insert(item, amount);

//        }
//      }

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

void Item::Container::set_name(std::string name)
{
  _name = name;
  set_modified();
}

void Item::Container::set_otable(dbTable otable)
{
  _otable = otable;
  set_modified();
}

void Item::Container::set_oref(dbRef oref)
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
void Item::Container::insert(std::shared_ptr<Item> &item, int amount)
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
    int amount_to_save = 0;

    if (item->isStackable() && _items.find(item->ref()) != _items.end() )
    {//update amount
      AmountedItem& aitem = _items[item->ref()];
      aitem.amount += amount;
      amount_to_save = aitem.amount;
    }
    else
    {//insert
      _items.insert(std::make_pair<dbRef, AmountedItem >(item->ref(), AmountedItem(item, amount)) );
      amount_to_save = amount;
    }

    save("EXECUTE PROCEDURE UPDINS_ITEM_CONTAINER_POS("+
          fun::toStr(ref())          +", "+ //container
          fun::toStr(item->ref())    +", "+ //item
          fun::toStr(amount_to_save) +")"   //amount
        );

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
    int amount_to_save = 0;

    if (!stackable) amount = 1;

    if (!stackable || cont_amount <= amount )
    {
      _items.erase(item_ref);
      amount_to_save = 0;
    }
    else
    {
      cont_amount -= amount;
      amount_to_save = cont_amount;
      ret.amount = amount;
    }

    save("EXECUTE PROCEDURE UPDINS_ITEM_CONTAINER_POS("+
          fun::toStr(ref())          +", "+ //container
          fun::toStr(item_ref)       +", "+ //item
          fun::toStr(amount_to_save) +")"   //amount
        );

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
             << " WHERE ref=" << ref();

  save(save_query.str());
  DBObject::saveToDB();
}

//===~~~

