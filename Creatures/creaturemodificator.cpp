#include "creaturemodificator.h"
#include "Include/functions/db_utils.h"
#include "Include/functions/string_utils.h"

using namespace std;
using namespace fun;

const dbTable CreatureModificator::tableName = "crt_mods";

void CreatureModificator::augument(const CreatureModificator &mod)
{
  _mods.augument(mod.creature_stats());
  _global_test_level_mod += mod.global_test_level_mod();
  set_modified();
}

void CreatureModificator::remove_augument(const CreatureModificator &mod)
{
  _mods.removeAugument(mod.creature_stats());
  _global_test_level_mod -= mod.global_test_level_mod();
  set_modified();
}

CreatureModificator::CreatureModificator(dbTable otable, dbRef oref)
  : DBObject(0)
  , _global_test_level_mod(0)
  , _effect_time(-1)
  , _otable(otable)
  , _oref(oref)
{

}

CreatureModificator::CreatureModificator(dbRef ref)
: DBObject(ref)
{
  load();
}

CreatureModificator::~CreatureModificator()
{
  _saveToDB_
}

void CreatureModificator::load(MapRow *data_source)
{
  if ( 0 != ref() )
  {
    MapRow mod_data;
    if (data_source != nullptr)
    {
      mod_data = *data_source;
    }
    else
    {
      mod_data = MapQuery("SELECT * FROM "+tableName+" WHERE ref="+toStr(ref()));
    }

    if (!mod_data.empty())
    {
      setName( CheckValue<string>(mod_data["NAME"]) );
      set_effect_time( CheckValue<int>(mod_data["EFFECT_TIME"]) );
      set_global_test_level_mod( CheckValue<int>(mod_data["GLB_TEST_MOD"]) );

      string atrs = CheckValue<string>(mod_data["ATTRIBUTES"]);
      if (!atrs.empty()) _mods.str2attributes(atrs);

      string skills = CheckValue<string>(mod_data["SKILLS"]);
      if (!skills.empty()) _mods.str2skills(skills);
    }

    set_loaded();
    set_not_modified();
  }
}

void CreatureModificator::saveToDB()
{
  if ( 0 == ref() )
  {    
    try
    {
      dbRef new_ref(0);
      soci::indicator ind;

      _Database << "SELECT ref FROM CREATE_EMPTY_CRT_MOD", soci::into(new_ref, ind);
      _Database.commit();

      if (soci::i_ok == ind)
      {
        setRef(new_ref);
        set_loaded();
        set_not_modified();
      }
    }
    catch(soci::soci_error &e)
    {
      qDebug() << "###Error saving " << table().c_str() << " ref=" << ref() << ": ";
      qDebug() << e.what();
      qDebug() << _Database.get_last_query().c_str();
    }
  }

  if ( 0 != ref() )
  {
    save("UPDATE " + tableName + " SET "
         " otable='" + _otable + "',"
         " oref=" + toStr(_oref) + ","
         " name='" + _name + "',"
         " effect_time=" + toStr(_effect_time) + ","
         " glb_test_mod=" + toStr(_global_test_level_mod) + ","
         " attributes='" + _mods.attributes2str() + "',"
         " skills='" + _mods.skills2str() + "'"
         " WHERE ref=" + toStr(ref()) );
  }

  DBObject::saveToDB();
}

