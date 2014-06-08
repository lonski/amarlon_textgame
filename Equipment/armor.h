#ifndef ARMOR_H
#define ARMOR_H

#include "item.h"

//==Armor
class Armor : public Item
{
private:
  friend class Item;
  Armor(dbRef ref);
public:
  virtual ~Armor() {}
};
//===

#endif // ARMOR_H
