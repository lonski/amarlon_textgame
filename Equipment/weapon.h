#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"
#include "../Include/comobj.h"

class Weapon : public Item
{
private:
  //data
  WeaponSkill _wpn_skill;
  Damage _damage;
  int _defence;
  int _attack;
  int _reflex;
  int _str_req;
  int _range;

  friend class Item;
  Weapon(dbRef ref, bool temporary = false);
public:
  //operations
  virtual void load(MapRow *data_source = nullptr);
  virtual void save_to_db();

  //data access
  WeaponSkill skill() const { return _wpn_skill; }
  Damage damage() const { return _damage; }
  int defence() const { return _defence; }
  int attack() const { return _attack; }
  int reflex() const { return _reflex; }
  int str_req() const { return _str_req; }
  int range() const { return _range; }

  //data set
  void set_skill(WeaponSkill skill);
  void set_damage(Damage damage);
  void set_defence(int defence);
  void set_attack(int attack);
  void set_reflex(int reflex);
  void set_str_req(int val);
  void set_range(int range);

  virtual ~Weapon();
};

#endif // WEAPON_H
