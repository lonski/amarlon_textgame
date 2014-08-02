#ifndef SHIELD_H
#define SHIELD_H

#include "item.h"

//==Shield
class Shield : public Item
{
private:
  //data
  int _defence;

  friend class Item;
  Shield(dbRef ref,bool temporary = false);
public:
  //operations
  virtual void load(MapRow *data_source = nullptr);
  virtual void saveToDB();

  //data access
  int defence() const { return _defence; }

  //data set
  void set_defence(int defence);

  virtual ~Shield();
};
//===

#endif // SHIELD_H
