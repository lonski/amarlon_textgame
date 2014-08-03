#ifndef FOOD_H
#define FOOD_H

#include "item.h"

class Food;
typedef std::shared_ptr<Food> FoodPtr;

class Food : public Item
{
public:

  virtual ~Food();

  virtual void load(MapRow *data_source = nullptr);
  virtual void saveToDB();

  int hunger() const { return _hunger; }

  void setHunger(int hunger);

  inline static Food* forge(ItemPrototype proto)
  {
    return dynamic_cast<Food*>(Item::prototypes().clone(proto));
  }

private:
  friend class Item;
  Food(dbRef ref,bool temporary = false);

  int _hunger;

};
//===

#endif // FOOD_H
