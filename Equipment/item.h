#ifndef ITEM_H
#define ITEM_H

#include "Include/inc.h"
#include "Include/common.h"
#include "Include/enums.h"
#include "Include/prototypemanager.h"

class Item : public DBObject
{
private:
  //data
  std::string _name;
  std::string _descript;
  double _weight;
  int _value;

  //operations
  virtual void load();

protected:
  Item(dbRef ref);

public:
  //creation
  static std::unique_ptr<Item> create(dbRef ref);
  static PrototypeManager<Item, ItemPrototypes> Prototypes;
  std::unique_ptr<Item> clone();

  //data access
  std::string name() const { return _name; }
  std::string descript() const { return _descript; }
  double weight() const { return _weight; }
  int value() const { return _value; }

  ~Item() = 0;
};

#endif // ITEM_H
