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
  std::string _name;
  std::string _descript;
  double _weight;
  int _value;
  ItemCondition _condition;
  int _durability;
  std::vector<BodyPartType> _body_parts;

  //deleted funcs
  Item& operator=(const Item&) = delete;
  Item(const Item&) = delete;
protected:
  Item(dbRef ref, bool temporary = false);

public:
  //parameters
  const static dbTable table_name;

  //creation
  static std::unique_ptr<Item> create(dbRef ref, bool prototype = false, bool temporary = false);
  std::unique_ptr<Item> clone();

  //operations
  virtual void load();

  //data access
  virtual dbTable table() const { return table_name; }
  ItemType type() const { return _item_type; }
  std::string name() const { return _name; }
  std::string descript() const { return _descript; }
  double weight() const { return _weight; }
  int value() const { return _value; }
  ItemCondition condition() const { return _condition; }
  int durability() const { return _durability; }
  std::vector<BodyPartType> body_parts() const { return _body_parts; }
  bool check_body_part(BodyPartType bp) const { return std::find(_body_parts.begin(), _body_parts.end(), bp) != _body_parts.end(); }

  //data set
  void set_type(ItemType type);
  void set_name(std::string name);
  void set_descript(std::string dsc);
  void set_weight(double weight);
  void set_value(int value);
  void set_condition(ItemCondition condition);
  void set_durability(int dura);
  void add_body_part(BodyPartType body_part);
  void remove_body_part(BodyPartType body_part);

  ~Item() = 0;
};
//===~~~

#endif // ITEM_H
