#ifndef FOOD_H
#define FOOD_H

#include "item.h"

//==Food
class Food : public Item
{
private:
  friend class Item;
  Food(dbRef ref);
public:
  virtual ~Food() {}
};
//===

#endif // FOOD_H
