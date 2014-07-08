#include "armor.h"

using namespace std;
using namespace soci;
using namespace fun;

Armor::Armor(dbRef ref, bool temporary): Item(ref, temporary)
{
}

void Armor::load(MapRow *data_source)
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
        Damage dmg
        (
          CheckField<int>(item_data["ARM_DR_PIERCING"]),
          CheckField<int>(item_data["ARM_DR_SLASHING"]),
          CheckField<int>(item_data["ARM_DR_BASHING"])
        );

        set_damage_reduction(dmg);

        Item::load(&item_data);
        set_not_modified();
      }

    }
    catch(soci_error &e)
    {
      MsgError(e.what());
      qDebug() << _Database.get_last_query().c_str();
    }
    }
}

void Armor::save_to_db()
{
  stringstream save_query;

  save_query << "UPDATE " << table() << " SET"
             << "  ARM_DR_PIERCING=" << _damage_red.piercing
             << " ,ARM_DR_SLASHING=" << _damage_red.slashing
             << " ,ARM_DR_BASHING=" << _damage_red.bashing
             << " WHERE ref=" << ref();

  save(save_query.str());
  Item::save_to_db();
}

void Armor::set_damage_reduction(Damage dmg_red)
{
  _damage_red = dmg_red;
  set_modified();
}

Armor::~Armor()
{
  _SAVE_TO_DB_
}
