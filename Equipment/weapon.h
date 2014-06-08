#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"

class Weapon : public Item
{
private:
  friend class Item;
  Weapon(dbRef ref);
public:
  virtual ~Weapon() {}
};

#endif // WEAPON_H
