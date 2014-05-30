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
  static PrototypeManager<Item, ItemPrototypes> *_prototypes;

  //data
  std::string _name;
  std::string _descript;
  double _weight;
  int _value;

protected:
  Item(dbRef ref);

public:
  //parameters
  const static dbTable table_name;

  //creation
  static std::unique_ptr<Item> create(dbRef ref);  
  static PrototypeManager<Item, ItemPrototypes>& prototypes();
  std::unique_ptr<Item> clone();

  //operations
  virtual void load();

  //data access
  virtual dbTable table() const { return table_name; }
  std::string name() const { return _name; }
  std::string descript() const { return _descript; }
  double weight() const { return _weight; }
  int value() const { return _value; }

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
