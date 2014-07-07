#include "armor.h"

using namespace std;
using namespace soci;
using namespace fun;

Armor::Armor(dbRef ref, bool temporary): Item(ref, temporary)
{
}

void Armor::load()
{
  if ( !loaded() && ref() > 0 ){
    try
    {
      MapRow item_data = MapQuery( "SELECT * FROM "+table()+" WHERE ref="+toStr(ref()) );
      if (!item_data.empty())
      {
        Damage dmg
        (
          CheckField<int>(item_data["ARM_DR_PIERCING"]),
          CheckField<int>(item_data["ARM_DR_SLASHING"]),
          CheckField<int>(item_data["ARM_DR_BASHING"])
        );

        set_damage_reduction(dmg);
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

void Armor::set_damage_reduction(Damage dmg_red)
{
  _damage_red = dmg_red;
  save("ARM_DR_PIERCING", _damage_red.piercing);
  save("ARM_DR_SLASHING", _damage_red.slashing);
  save("ARM_DR_BASHING", _damage_red.bashing);
}
