#include "weapon.h"

using namespace std;
using namespace soci;
using namespace fun;

Weapon::Weapon(dbRef ref, bool temporary): Item(ref, temporary)
{
}

void Weapon::load()
{
  if ( !loaded() && ref() > 0 ){
    try
    {
      MapRow item_data = MapQuery( "SELECT * FROM "+table()+" WHERE ref="+toStr(ref()) );
      if (item_data.size() > 0)
      {
        set_skill(CheckFieldCast<WeaponSkill>(item_data["WPN_SKILL"]));
        set_defence(CheckField<int>(item_data["WPN_DEFENCE"]));
        set_attack(CheckField<int>(item_data["WPN_ATTACK"]));
        set_reflex(CheckField<int>(item_data["WPN_REFLEX"]));
        set_str_req(CheckField<int>(item_data["WPN_STR_REQ"]));
        set_range(CheckField<int>(item_data["WPN_RANGE"]));

        Damage dmg
        (
          CheckField<int>(item_data["WPN_D_PIERCING"]),
          CheckField<int>(item_data["WPN_D_SLASHING"]),
          CheckField<int>(item_data["WPN_D_BASHING"])
        );

        set_damage(dmg);

      }
    }
    catch(soci_error &e)
    {
      MsgError(e.what());
      qDebug() << _Database.get_last_query().c_str();
    }
  }

  Item::load();
}

void Weapon::set_skill(WeaponSkill skill)
{
  _wpn_skill = skill;
  save("WPN_SKILL",static_cast<int>(_wpn_skill));
}

void Weapon::set_damage(Damage damage)
{
  _damage = damage;
  save("WPN_D_PIERCING", _damage.piercing);
  save("WPN_D_SLASHING", _damage.slashing);
  save("WPN_D_BASHING", _damage.bashing);
}

void Weapon::set_defence(int defence)
{
  _defence = defence;
  save("WPN_DEFENCE", _defence);
}

void Weapon::set_attack(int attack)
{
  _attack = attack;
  save("WPN_ATTACK", _attack);
}

void Weapon::set_reflex(int reflex)
{
  _reflex = reflex;
  save("WPN_REFLEX", _reflex);
}

void Weapon::set_str_req(int val)
{
  _str_req = val;
  save("WPN_STR_REQ", _str_req);
}

void Weapon::set_range(int range)
{
  _range = range;
  save("WPN_RANGE", _range);
}
