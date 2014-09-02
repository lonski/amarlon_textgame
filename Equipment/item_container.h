#ifndef ITEM_CONTAINER_H
#define ITEM_CONTAINER_H

#include "item.h"
#include "Include/custom_values.h"

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

public:
  const static dbTable tableName;
  const static int defaultCapacity = 10;
  static DataGateway* containersGateway;

  Container(dbRef);
  ~Container();
  static Container* create(dbRef = 0);
  static dbRef byOwner(dbTable otable, dbRef oref);

  virtual void load(MapRow *data_source = nullptr);
  virtual void saveToDB();
  void insert(ItemPtr& item, int amount = 1);
  AmountedItem erase(dbRef item_ref, int amount = 1);
  AmountedItem find(dbRef item_ref);
  std::vector<AmountedItem> getAll();
  AmountedItem get(size_t index);
  size_t count();

  virtual dbTable table() const { return tableName; }
  std::string name() const { return _name; }
  dbTable otable() const { return _otable; }
  dbRef oref() const { return _oref; }
  CMValue<Weight> weight_capacity() const { return _weight_cap; }

  void setName(std::string name);
  void setOTable(dbTable otable);
  void setORef(dbRef oref);
  void set_max_weight(Weight max_weight);

  std::string getItemsString();
  void setItems(std::string items);

};

#endif // ITEM_CONTAINER_H
