#ifndef JEWELRY_H
#define JEWELRY_H

#include "item.h"

//==Jewelry
class Jewelry : public Item
{
private:
  friend class Item;
  Jewelry(dbRef ref);
public:
  virtual ~Jewelry() {}
};
//===

#endif // JEWELRY_H
