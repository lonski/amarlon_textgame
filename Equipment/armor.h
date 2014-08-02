#ifndef ARMOR_H
#define ARMOR_H

#include "item.h"
#include "../Include/comobj.h"

//==Armor
class Armor : public Item
{
private:
  //data
  Damage _damage_red;

  friend class Item;
  Armor(dbRef ref, bool temporary = false);
  friend class TestCreature;
public:
  //operations
  virtual void load(MapRow *data_source = nullptr);
  virtual void saveToDB();

  //data access
  Damage damage_reduction() const { return _damage_red; }

  //data set
  void set_damage_reduction(Damage dmg_red);

  virtual ~Armor();

  inline static Armor* Forge(ItemPrototype proto)
  {
    return dynamic_cast<Armor*>(Item::prototypes().clone(proto).release());
  }
};
//===

#endif // ARMOR_H
