#ifndef ITEM_H
#define ITEM_H

#include "Include/inc.h"
#include "Include/db.h"
#include "Include/enums.h"
#include "Include/prototypemanager.h"
#include "Include/exceptions.h"

//===Item
class Item : public DBObject, public Prototypable<Item, ItemPrototype>
{
private:
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

#endif // ITEM_H
