#ifndef ARMOR_H
#define ARMOR_H

#include "item.h"
#include "../Include/comobj.h"

class Armor;
typedef std::shared_ptr<Armor> ArmorPtr;

class Armor : public Item
{
public:
  virtual ~Armor();

  virtual void load(MapRow *data_source = nullptr);
  virtual void saveToDB();

  Damage damageReduction() const { return _damage_red; }

  void setDamageReduction(Damage dmg_red);

  inline static Armor* forge(ItemPrototype proto)
  {
    return dynamic_cast<Armor*>(Item::prototypes().clone(proto));
  }

private:
  friend class TestCreature;
  friend class Item;

  Armor(dbRef ref, bool temporary = false);

  Damage _damage_red;

};

#endif // ARMOR_H
