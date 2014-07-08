#ifndef ITEM_H
#define ITEM_H

#include "Include/inc.h"
#include "Include/db.h"
#include "Include/enums.h"
#include "Include/prototypemanager.h"
#include "Include/exceptions.h"
#include "Include/comobj.h"
#include "Creatures/creaturemodificator.h"

class Item;

template<typename T = Item>
struct AmountedItem
{
  std::shared_ptr<T> item;
  int amount;
  AmountedItem(std::shared_ptr<T> i_item = std::shared_ptr<T>(nullptr), int i_amount = 1) : item(i_item), amount(i_amount) {}
};

//===Item
class Item : public DBObject, public Prototypable<Item, ItemPrototype>
{
public:  
  //===CONTAINER===
  template<typename T = Item>
  class Container : public DBObject, public Prototypable<Container<T>, ItemContainerPrototype>
  {
  private:
    //data
    std::string _name;
    dbTable _otable;
    dbRef _oref;
    CMValue<Weight> _weight_cap;

    //items
    std::map<dbRef, AmountedItem<T> > _items;

  protected:
    Container(dbRef ref, bool temporary = false);

  public:
    //parameters
    const static dbTable table_name;

    //creation
    static dbRef byOwner(dbTable otable, dbRef oref);
    static std::unique_ptr<Container> create(dbRef ref, bool prototype = false, bool temporary = false);
    std::unique_ptr<Container> clone();
    ~Container();

    //general & items operations
    virtual void load(MapRow *data_source = nullptr);
    virtual void save_to_db();
    void insert(std::shared_ptr<T>& item, int amount = 1);
    AmountedItem<T> erase(dbRef item_ref, int amount = 1);
    AmountedItem<T> find(dbRef item_ref);
    std::vector<AmountedItem<T> > get_all();

    //cont header data access
    virtual dbTable table() const { return table_name; }
    std::string name() const { return _name; }
    dbTable otable() const { return _otable; }
    dbRef oref() const { return _oref; }
    CMValue<Weight> weight_capacity() const { return _weight_cap; }

    //cont hader data set
    void set_name(std::string name);
    void set_otable(dbTable otable);
    void set_oref(dbRef oref);
    void set_max_weight(Weight max_weight);

  };
  //===~~~
public:
  typedef std::unique_ptr<Item::Container<> > Inventory;
  typedef std::vector<std::shared_ptr<Item> > STLContainer;

private:
  //data
  ItemType _item_type;
  std::string _name;
  std::string _descript;
  Weight _weight;
  int _value;
  ItemCondition _condition;
  int _durability;
  std::vector<BodyPartType> _body_parts;
  bool _stackable;

  Inventory _inventory;
  CreatureModificatorManager _mods;

  //deleted funcs
  Item& operator=(const Item&) = delete;
  Item(const Item&) = delete;
protected:
  Item(dbRef ref, bool temporary = false);

public:   
  //parameters
  const static dbTable table_name;
  virtual dbTable table() const { return table_name; }

  //creation  
  static std::unique_ptr<Item> create(dbRef ref, bool prototype = false, bool temporary = false);
  std::unique_ptr<Item> clone();
  virtual ~Item() = 0;

  //operations
  virtual void load(MapRow *data_source = nullptr);
  virtual void save_to_db();

  //inventory
  Inventory& inventory() { return _inventory; }

  //creature modificators
  CreatureModificatorManager& mods() { return _mods; }

  //data access  
  ItemType type() const { return _item_type; }
  std::string name() const { return _name; }
  std::string descript() const { return _descript; }
  Weight weight() const { return _weight; }
  int value() const { return _value; }
  ItemCondition condition() const { return _condition; }
  int durability() const { return _durability; }
  std::vector<BodyPartType> body_parts() const { return _body_parts; }
  bool check_body_part(BodyPartType bp) const { return std::find(_body_parts.begin(), _body_parts.end(), bp) != _body_parts.end(); }
  bool isStackable() const { return _stackable; }

  //data set
  void set_type(ItemType type);
  void set_name(std::string name);
  void set_descript(std::string dsc);
  void set_weight(Weight weight);
  void set_value(int value);
  void set_condition(ItemCondition condition);
  void set_durability(int dura);
  void add_body_part(BodyPartType body_part);
  void remove_body_part(BodyPartType body_part);
  void set_stackable(bool stackable);

};
//===~~~

/* Container typedefs */
class OrdinaryItem;
class Weapon;
class Armor;
class Shield;
class Jewelry;
class Food;
class LocationObject;

typedef Item::Container<> ItemContainer;
typedef Item::Container<OrdinaryItem> OItemContainer;
typedef Item::Container<Weapon> WeaponContainer;
typedef Item::Container<Armor> ArmorContainer;
typedef Item::Container<Shield> ShieldContainer;
typedef Item::Container<Jewelry> JewelryContainer;
typedef Item::Container<Food> FoodContainer;
typedef Item::Container<LocationObject> LocationObjectContainer;

//=============================ITEM CONTAINER DEF=========================
template<typename T>
const dbTable Item::Container<T>::table_name = "item_container_def";

template<typename T>
Item::Container<T>::Container(dbRef ref, bool temporary) : DBObject(ref, temporary)
{
}

template<typename T>
Item::Container<T>::~Container()
{
  _SAVE_TO_DB_
}

template<typename T>
dbRef Item::Container<T>::byOwner(dbTable otable, dbRef oref)
{
  dbRef ref = 0;
  soci::indicator ind;
  _Database << "SELECT ref FROM item_container_def WHERE otable='"<<otable<<"' and oref="<<fun::toStr(oref), into(ref, ind);
  if (ind != soci::i_ok) ref = 0;

  return ref;
}

template<typename T>
std::unique_ptr<Item::Container<T> > Item::Container<T>::create(dbRef ref, bool prototype, bool temporary)
{
  Container<T>* new_container = nullptr;

  if (ref > 0)
  {
    MapRow cont_data = fun::MapQuery("SELECT obj_type FROM "+table_name+" WHERE ref="+fun::toStr(ref));
    ObjType obj_type = fun::CheckFieldCast<ObjType>( cont_data["OBJ_TYPE"] );

    if ( obj_type == ObjType::Instance || prototype )
    {
        new_container = new Container<T>(ref, temporary);
    }else throw error::creation_error("Brak prawidłowego rekordu w bazie.");

    new_container->load();
  }

  return std::unique_ptr<Item::Container<T> >(new_container);
}

template<typename T>
void Item::Container<T>::load(MapRow *data_source)
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
        cont_data = fun::MapQuery("SELECT name, otable, oref, max_weight FROM "+table_name+" WHERE ref="+fun::toStr(ref()));
      }

      //==header data
      if (!cont_data.empty())
      {
        set_name( fun::CheckField<std::string>(cont_data["NAME"]) );
        set_max_weight( fun::CheckField<Weight>(cont_data["MAX_WEIGHT"]) );
        set_oref( fun::CheckField<dbRef>(cont_data["OREF"]) );
        set_otable( fun::CheckField<std::string>(cont_data["OTABLE"]) );        
      }

      //==pos data
      _items.clear();
      MapTable pos_data;
      fun::MapQuery("SELECT * FROM item_container_pos WHERE container="+fun::toStr(ref()), pos_data);
      if (pos_data.size() > 0)
      {
        for (auto RowIter = pos_data.begin(); RowIter != pos_data.end(); ++RowIter)
        {
          dbRef item_ref = fun::CheckField<dbRef>( (*RowIter)["ITEM"] );
          int amount = fun::CheckField<int>( (*RowIter)["AMOUNT"] );

          std::shared_ptr<T> item( dynamic_cast<T*>(Item::create(item_ref).release()) );
          insert(item, amount);

        }
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

template<typename T>
std::unique_ptr<Item::Container<T> > Item::Container<T>::clone()
{
  if (!isTemporary())
  {
    //save
    save_to_db();

    //clone db record
    dbRef new_ref(0);
    _Database << "EXECUTE PROCEDURE CLONE_ITEM_CONTAINER("<< ref() << ")", soci::into(new_ref);
    _Database.commit();

    //return new item
    return Item::Container<T>::create(new_ref);
  }

  return std::unique_ptr<Item::Container<T> >(nullptr);
}

template<typename T>
void Item::Container<T>::set_name(std::string name)
{
  _name = name;
  set_modified();
}

template<typename T>
void Item::Container<T>::set_otable(dbTable otable)
{
  _otable = otable;
  set_modified();
}

template<typename T>
void Item::Container<T>::set_oref(dbRef oref)
{
  _oref = oref;
  set_modified();
}

template<typename T>
void Item::Container<T>::set_max_weight(Weight max_weight)
{
  _weight_cap.max = max_weight;
  set_modified();
}

/*
 *  Passing a reference (not by value), because otherwise if insert fail, the item could be lost.
 */
template<typename T>
void Item::Container<T>::insert(std::shared_ptr<T> &item, int amount)
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
      AmountedItem<T>& aitem = _items[item->ref()];
      aitem.amount += amount;
      amount_to_save = aitem.amount;
    }
    else
    {//insert
      _items.insert(std::make_pair<dbRef, AmountedItem<T> >(item->ref(), AmountedItem<T>(item, amount)) );
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

template<typename T>
AmountedItem<T> Item::Container<T>::erase(dbRef item_ref, int amount)
{

  auto iter = _items.find(item_ref);
  AmountedItem<T> ret;
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

template<typename T>
AmountedItem<T> Item::Container<T>::find(dbRef item_ref)
{
  AmountedItem<T> ret;
  auto iter = _items.find(item_ref);
  if (iter != _items.end()) ret = iter->second;

  return ret;
}

template<typename T>
std::vector<AmountedItem<T> > Item::Container<T>::get_all()
{
  std::vector<AmountedItem<T> > ret;
  ret.reserve(_items.size());

  for (auto iter = _items.begin(); iter != _items.end(); ++iter)
    ret.push_back(iter->second);

  return ret;
}

template<typename T>
void Item::Container<T>::save_to_db()
{
  std::stringstream save_query;

  save_query << "UPDATE " << table() << " SET"
             << "  MAX_WEIGHT=" << _weight_cap.max
             << " ,NAME='" << _name << "'"
             << " ,OREF=" << _oref
             << " ,OTABLE='" << _otable << "'"
             << " WHERE ref=" << ref();

  save(save_query.str());
  DBObject::save_to_db();
}

//===~~~

#endif // ITEM_H
