#ifndef ITEM_H
#define ITEM_H

#include "Include/inc.h"
#include "Include/db.h"
#include "Include/enums.h"
#include "Include/prototypemanager.h"
#include "Include/exceptions.h"
#include "Include/comobj.h"
#include "Creatures/creaturemodificator.h"

#define _ForgeFood(PROTOTYPE) dynamic_cast<Food*>(Item::prototypes().clone(PROTOTYPE).release())

class Item;

struct AmountedItem
{
  std::shared_ptr<Item> item;
  int amount;
  AmountedItem(std::shared_ptr<Item> i_item = std::shared_ptr<Item>(nullptr), int i_amount = 1) : item(i_item), amount(i_amount) {}
};

//===Item
class Item : public DBObject, public Prototypable<Item, ItemPrototype>
{
public:  
  //===CONTAINER===
  class Container : public DBObject
  {
  private:
    //data
    std::string _name;
    dbTable _otable;
    dbRef _oref;
    CMValue<Weight> _weight_cap;

    //items
    std::map<dbRef, AmountedItem> _items;

  public:
    const static dbTable table_name;

    Container(dbRef ref, bool temporary = false);
    Container();
    ~Container();
    static dbRef byOwner(dbTable otable, dbRef oref);

    virtual void load(MapRow *data_source = nullptr);
    virtual void saveToDB();
    void insert(std::shared_ptr<Item>& item, int amount = 1);
    AmountedItem erase(dbRef item_ref, int amount = 1);
    AmountedItem find(dbRef item_ref);
    std::vector<AmountedItem> getAll();

    virtual dbTable table() const { return table_name; }
    std::string name() const { return _name; }
    dbTable otable() const { return _otable; }
    dbRef oref() const { return _oref; }
    CMValue<Weight> weight_capacity() const { return _weight_cap; }

    void set_name(std::string name);
    void set_otable(dbTable otable);
    void set_oref(dbRef oref);
    void set_max_weight(Weight max_weight);
  };
  //===~~~

public:
  typedef std::shared_ptr<Item::Container > Inventory;
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
  virtual void saveToDB();

  //inventory
  Inventory& inventory();

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

  inline static Item* Forge(ItemPrototype proto)
  {
    return Item::prototypes().clone(proto).release();
  }
};
//===~~~

class OrdinaryItem;
class Weapon;
class Armor;
class Shield;
class Jewelry;
class Food;
class LocationObject;

/* item pointers typdefs */
typedef std::shared_ptr<Item> ItemPtr;
typedef std::shared_ptr<Shield> ShieldPtr;
typedef std::shared_ptr<Armor> ArmorPtr;
typedef std::shared_ptr<Weapon> WeaponPtr;
typedef std::shared_ptr<Food> FoodPtr;
typedef std::shared_ptr<Jewelry> JewelryPtr;
typedef std::shared_ptr<LocationObject> LocationObjectPtr;

#endif // ITEM_H
