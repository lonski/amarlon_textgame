#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"
#include "../Include/comobj.h"

#include "Include/enums/e_weaponskill.h"

class Weapon;
typedef std::shared_ptr<Weapon> WeaponPtr;

class Weapon : public Item
{
public:
  virtual ~Weapon();

  virtual void load(MapRow *data_source = nullptr);
  virtual void saveToDB();

  WeaponSkill skill() const { return _wpn_skill; }
  Damage damage() const { return _damage; }
  int defence() const { return _defence; }
  int attack() const { return _attack; }
  int reflex() const { return _reflex; }
  int str_req() const { return _str_req; }
  int range() const { return _range; }

  void setSkill(WeaponSkill skill);
  void setDamage(Damage damage);
  void setDefence(int defence);
  void setAttack(int attack);
  void setReflex(int reflex);
  void setStrReq(int val);
  void setRange(int range);

  inline static Weapon* forge(ItemPrototype proto)
  {
    return dynamic_cast<Weapon*>(Item::prototypes().clone(proto));
  }

private:
  friend class Item;
  Weapon(dbRef ref, bool temporary = false);

  WeaponSkill _wpn_skill;
  Damage _damage;
  int _defence;
  int _attack;
  int _reflex;
  int _str_req;
  int _range;

};

#endif // WEAPON_H
