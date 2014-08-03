#ifndef ITEM_CONTAINER_H
#define ITEM_CONTAINER_H

#include "item.h"

class Item::Container : public DBObject
{
private:
  //data
  std::string _name;
  dbTable _otable;
  dbRef _oref;
  CMValue<Weight> _weight_cap;

  //items
  std::map<dbRef, AmountedItem> _items;
  std::string items2str();
  void str2items(std::string items);

public:
  const static dbTable tableName;

  Container(dbRef ref, bool temporary = false);
  Container(int pojemnosc = 10);
  ~Container();
  static dbRef byOwner(dbTable otable, dbRef oref);

  virtual void load(MapRow *data_source = nullptr);
  virtual void saveToDB();
  void insert(ItemPtr& item, int amount = 1);
  AmountedItem erase(dbRef item_ref, int amount = 1);
  AmountedItem find(dbRef item_ref);
  std::vector<AmountedItem> getAll();

  virtual dbTable table() const { return tableName; }
  std::string name() const { return _name; }
  dbTable otable() const { return _otable; }
  dbRef oref() const { return _oref; }
  CMValue<Weight> weight_capacity() const { return _weight_cap; }

  void setName(std::string name);
  void set_otable(dbTable otable);
  void set_oref(dbRef oref);
  void set_max_weight(Weight max_weight);
};

#endif // ITEM_CONTAINER_H
