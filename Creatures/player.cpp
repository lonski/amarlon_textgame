#include "player.h"

using namespace std;
using namespace fun;
using namespace soci;

const dbRef Player::player_ref = 5;
Player* Player::_instance = nullptr;

Player::Player() : Creature(player_ref)
{
}

Player* Player::inst()
{
  if (_instance == nullptr)
  {
    _instance = new Player;
    _instance->load();
  }

  return _instance;
}

void Player::load(MapRow *data_source)
{
  if ( !loaded() && ref() > 0 )
  {
    try
    {      
      //DATA
      MapRow plr_data;
      if (data_source != nullptr)
      {
        plr_data = *data_source;
      }
      else
      {
        plr_data = MapQuery( "SELECT first 1 p.*, c.tribe, c.clan FROM player p join creatures c on (c.ref = "+fun::toStr(Player::player_ref)+")" );
      }

      if (!plr_data.empty())
      {
        //base data
        _prof = CheckFieldCast<Profession>(plr_data["PROFESSION"]);
        _bless = CheckFieldCast<Bless>(plr_data["BLESS"]);
        _splot = CheckField<Splot>(plr_data["SPLOT"]);
        _birth_season = CheckFieldCast<Season>(plr_data["BIRTH_SEASON"]);
        _tribe = CheckFieldCast<Tribe>(plr_data["TRIBE"]);
        _clan = CheckFieldCast<Clan>(plr_data["CLAN"]);
        _hunger = CheckFieldCast<HungerLevel>(plr_data["HUNGER"]);
        _fatigue = CheckFieldCast<FatigueLevel>(plr_data["FATIGUE"]);
        _brave = CheckField<BravePoints>(plr_data["BRAVE"]);
        _lift_cap.cur = 0;
        _lift_cap.max = CheckField<int>(plr_data["LIFT_CAP"]);

        string tmp;
        vector<string> vtmp;
        tmp = CheckField<string>(plr_data["EXP"]);
        vtmp = fun::explode(tmp,'/');
        if (vtmp.size() == 2)
        {
          _exp.left = fun::fromStr<int>(vtmp[0]);
          _exp.val = fun::fromStr<int>(vtmp[1]);          
        }

        tmp = CheckField<string>(plr_data["FAME"]);
        vtmp = fun::explode(tmp,'/');
        if (vtmp.size() == 2)
        {
          _fame.left = fun::fromStr<int>(vtmp[0]);
          _fame.val = fun::fromStr<int>(vtmp[1]);
        }
      }

      Creature::load(&plr_data);
    }
    catch(soci_error &e)
    {
      MsgError(e.what());
      qDebug() << _Database.get_last_query().c_str();
    }
  }
}

void Player::save_to_db()
{
  stringstream save_query;

  save_query << "UPDATE player SET "
             << "  profession=" << static_cast<int>(_prof)
             << " ,bless=" << static_cast<int>(_bless)
             << " ,splot=" << static_cast<int>(_splot)
             << " ,birth_season=" << static_cast<int>(_birth_season)
             << " ,hunger=" << static_cast<int>(_hunger)
             << " ,fatigue=" << static_cast<int>(_fatigue)
             << " ,brave=" << static_cast<int>(_brave)
             << " ,lift_cap=" << _lift_cap.max
             << " ,exp='" << _exp.left << "/" << _exp.val << "'"
             << " ,fame='" << _fame.left << "/" << _fame.val << "'"
             << " WHERE ref = 1";

  save(save_query.str());
  save_query.str("");
  save_query.flush();

  save_query << "UPDATE " << table() << " SET "
             << "  clan=" << static_cast<int>(_clan)
             << " ,tribe=" << static_cast<int>(_tribe)
             << " WHERE ref = " << ref();

  save(save_query.str());

  DBObject::save_to_db();
}
