#include "npc.h"

using namespace std;
using namespace fun;
using namespace soci;

NPC::NPC(dbRef ref, bool temporary): Creature(ref, temporary)
{
}

void NPC::set_clan(Clan clan)
{
  _clan = clan;
  set_modified();
}

void NPC::set_tribe(Tribe tribe)
{
  _tribe = tribe;
  set_modified();
}

void NPC::saveToDB()
{
  stringstream save_query;

  save_query << "UPDATE " << table() << " SET "
             << "  clan=" << static_cast<int>(_clan)
             << " ,tribe=" << static_cast<int>(_tribe)
             << " WHERE ref = " << ref();

  save(save_query.str());
  Creature::saveToDB();
}

void NPC::load(MapRow *data_source)
{
  if ( !loaded() && ref() > 0 )
  {
    try
    {
      //DATA
      MapRow crt_data;
      if (data_source != nullptr)
      {
        crt_data = *data_source;
      }
      else
      {
        crt_data = MapQuery( "SELECT * FROM "+table()+" WHERE ref="+toStr(ref()) );
      }

      if (!crt_data.empty())
      {
        //base data
        set_clan( CheckFieldCast<Clan>(crt_data["CLAN"]) );
        set_tribe( CheckFieldCast<Tribe>(crt_data["TRIBE"]) );
      }

      Creature::load(&crt_data);
    }
    catch(soci_error &e)
    {
      MsgError(e.what());
      qDebug() << _Database.get_last_query().c_str();
    }
  }
}

NPC::~NPC()
{
  _saveToDB_
}
