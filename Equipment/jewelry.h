#ifndef JEWELRY_H
#define JEWELRY_H

#include "item.h"

//==Jewelry
class Jewelry : public Item
{
private:
  friend class Item;
  Jewelry(dbRef ref, bool temporary = false);
public:
  virtual ~Jewelry() {}

  inline static Jewelry* Forge(ItemPrototype proto)
  {
    return dynamic_cast<Jewelry*>(Item::prototypes().clone(proto).release());
  }
};
//===

#endif // JEWELRY_H
