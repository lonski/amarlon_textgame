#ifndef JEWELRY_H
#define JEWELRY_H

#include "item.h"

class Jewelry;
typedef std::shared_ptr<Jewelry> JewelryPtr;

class Jewelry : public Item
{
public:
  virtual ~Jewelry() {}

  inline static Jewelry* forge(ItemPrototype proto)
  {
    return dynamic_cast<Jewelry*>(Item::prototypes().clone(proto));
  }

private:
  friend class Item;
  Jewelry(dbRef ref, bool temporary = false);

};

#endif // JEWELRY_H
