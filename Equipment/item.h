#ifndef ITEM_H
#define ITEM_H

#include "Include/inc.h"
#include "Include/db.h"
#include "Include/enums.h"
#include "Include/prototypemanager.h"
#include "Include/exceptions.h"

//===Item
class Item : public DBObject
{
private:
  static PrototypeManager<Item, ItemPrototype> *_prototypes;

  //data
  ItemType _item_type;
  ItemSizeClass _size_class;
  BodyPart _body_part;
  std::string _name;
  std::string _descript;
  double _weight;
  int _value;

  //deleted funcs
  Item& operator=(const Item&) = delete;
  Item(const Item&) = delete;
protected:
  Item(dbRef ref);

public:
  //parameters
  const static dbTable table_name;

  //creation
  static std::unique_ptr<Item> create(dbRef ref, bool prototype = false);
  static PrototypeManager<Item, ItemPrototype>& prototypes();
  std::unique_ptr<Item> clone();

  //operations
  virtual void load();

  //data access
  virtual dbTable table() const { return table_name; }
  ItemType item_type() const { return _item_type; }
  ItemSizeClass size_class() const { return _size_class; }
  BodyPart body_part() const { return _body_part; }
  std::string name() const { return _name; }
  std::string descript() const { return _descript; }
  double weight() const { return _weight; }
  int value() const { return _value; }

  //data set
  void set_item_type(ItemType type);
  void set_size_class(ItemSizeClass size_class);
  void set_body_part(BodyPart body_part);
  void set_name(std::string name);
  void set_destript(std::string dsc);
  void set_weight(double weight);
  void set_value(int value);

  ~Item() = 0;
};
//===~~~

//==Ordinary Item
class OrdinaryItem : public Item
{
private:
  friend class Item;
  OrdinaryItem(dbRef ref): Item(ref) {}
public:
  virtual ~OrdinaryItem() {}
};
//===

//==Weapon
class Weapon : public Item
{
private:
  friend class Item;
  Weapon(dbRef ref): Item(ref) {}
public:
  virtual ~Weapon() {}
};
//===

//==Armor
class Armor : public Item
{
private:
  friend class Item;
  Armor(dbRef ref): Item(ref) {}
public:
  virtual ~Armor() {}
};
//===

//==Jewelry
class Jewelry : public Item
{
private:
  friend class Item;
  Jewelry(dbRef ref): Item(ref) {}
public:
  virtual ~Jewelry() {}
};
//===

//==Food
class Food : public Item
{
private:
  friend class Item;
  Food(dbRef ref): Item(ref) {}
public:
  virtual ~Food() {}
};
//===

//==Food
class Tool : public Item
{
private:
  friend class Item;
  Tool(dbRef ref): Item(ref) {}
public:
  virtual ~Tool() {}
};
//===

#endif // ITEM_H
