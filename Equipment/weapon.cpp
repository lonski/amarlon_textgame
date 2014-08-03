#include "weapon.h"

using namespace std;
using namespace soci;
using namespace fun;

Weapon::Weapon(dbRef ref, bool temporary): Item(ref, temporary)
{
}

void Weapon::load(MapRow *data_source)
{
  if ( !loaded() && ref() > 0 ){
    try
    {
      MapRow item_data;
      if (data_source != nullptr)
      {
        item_data = *data_source;
      }
      else
      {
        item_data = MapQuery( "SELECT * FROM "+table()+" WHERE ref="+toStr(ref()) );
      }

      if (!item_data.empty())
      {
        setSkill(CheckFieldCast<WeaponSkill>(item_data["WPN_SKILL"]));
        setDefence(CheckField<int>(item_data["WPN_DEFENCE"]));
        setAttack(CheckField<int>(item_data["WPN_ATTACK"]));
        setReflex(CheckField<int>(item_data["WPN_REFLEX"]));
        setStrReq(CheckField<int>(item_data["WPN_STR_REQ"]));
        setRange(CheckField<int>(item_data["WPN_RANGE"]));

        Damage dmg
        (
          CheckField<int>(item_data["WPN_D_PIERCING"]),
          CheckField<int>(item_data["WPN_D_SLASHING"]),
          CheckField<int>(item_data["WPN_D_BASHING"])
        );

        setDamage(dmg);

      }

      Item::load(&item_data);
      set_not_modified();
    }
    catch(soci_error &e)
    {
      MsgError(e.what());
      qDebug() << _Database.get_last_query().c_str();
    }
  }
}

void Weapon::saveToDB()
{
  stringstream save_query;

  save_query << "UPDATE " << table() << " SET"
             << "  WPN_SKILL=" << static_cast<int>(_wpn_skill)
             << " ,WPN_D_PIERCING=" << _damage.piercing
             << " ,WPN_D_SLASHING=" << _damage.slashing
             << " ,WPN_D_BASHING=" << _damage.bashing
             << " ,WPN_DEFENCE=" << _defence
             << " ,WPN_ATTACK=" << _attack
             << " ,WPN_REFLEX=" << _reflex
             << " ,WPN_STR_REQ=" << _str_req
             << " ,WPN_RANGE=" << _range
             << " WHERE ref=" << ref();

  save(save_query.str());
  Item::saveToDB();
}

void Weapon::setSkill(WeaponSkill skill)
{
  _wpn_skill = skill;
  set_modified();
}

void Weapon::setDamage(Damage damage)
{
  _damage = damage;
  set_modified();
}

void Weapon::setDefence(int defence)
{
  _defence = defence;
  set_modified();
}

void Weapon::setAttack(int attack)
{
  _attack = attack;
  set_modified();
}

void Weapon::setReflex(int reflex)
{
  _reflex = reflex;
  set_modified();
}

void Weapon::setStrReq(int val)
{
  _str_req = val;
  set_modified();
}

void Weapon::setRange(int range)
{
  _range = range;
  set_modified();
}

Weapon::~Weapon()
{
  _saveToDB_
}
