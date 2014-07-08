#ifndef FOOD_H
#define FOOD_H

#include "item.h"

//==Food
class Food : public Item
{
private:
  //data
  int _hunger;

  friend class Item;
  Food(dbRef ref,bool temporary = false);
public:
  //operations
  virtual void load(MapRow *data_source = nullptr);
  virtual void save_to_db();


  //data access
  int hunger() const { return _hunger; }

  //data set
  void set_hunger(int hunger);

  virtual ~Food();
};
//===

#endif // FOOD_H
