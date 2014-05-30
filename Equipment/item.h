#ifndef ITEM_H
#define ITEM_H

#include "Include/inc.h"
#include "Include/common.h"
#include "Include/enums.h"

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
  //prototypes
  static class PrototypeManager
  {
  private:
    std::map<std::unique_ptr<Item>, ItemPrototypes> _prototypes;
    void load_all();
  public:
    std::unique_ptr<Item> clone(ItemPrototypes key);
  } Prototypes;

  //creation
  static std::unique_ptr<Item> create(dbRef ref);
  std::unique_ptr<Item> clone();

  //data access
  std::string name() const { return _name; }
  std::string descript() const { return _descript; }
  double weight() const { return _weight; }
  int value() const { return _value; }

  ~Item() {}
};

#endif // ITEM_H
