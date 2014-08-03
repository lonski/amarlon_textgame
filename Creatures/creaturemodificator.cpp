#include "creaturemodificator.h"

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
  _mods.remove_augument(mod.creature_stats());
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

CreatureModificator::CreatureModificator(dbRef ref, bool temporary)
: DBObject(ref, temporary)
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
      setName( CheckField<string>(mod_data["NAME"]) );
      set_effect_time( CheckField<int>(mod_data["EFFECT_TIME"]) );
      set_global_test_level_mod( CheckField<int>(mod_data["GLB_TEST_MOD"]) );

      string atrs = CheckField<string>(mod_data["ATTRIBUTES"]);
      if (!atrs.empty()) _mods.Str2Attributes(atrs);

      string skills = CheckField<string>(mod_data["SKILLS"]);
      if (!skills.empty()) _mods.Str2Skills(skills);
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
        set_ref(new_ref);
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
         " attributes='" + _mods.Attributes2Str() + "',"
         " skills='" + _mods.Skills2Str() + "'"
         " WHERE ref=" + toStr(ref()) );
  }

  DBObject::saveToDB();
}

//==================================CreatureModificatorManager====================================
CreatureModificatorManager::CreatureModificatorManager(DBObject *owner)
: _complex_mod( shared_ptr<CreatureModificator>(new CreatureModificator) )
, _owner(owner)
{
  if (owner != nullptr)
  {
    _complex_mod->set_oref(owner->ref());
    _complex_mod->set_otable(owner->table());    
  }
}

CreatureModificatorManager::~CreatureModificatorManager()
{
  _complex_mod->purge();
}

void CreatureModificatorManager::add(std::shared_ptr<CreatureModificator> new_mod)
{
  if (0 == new_mod->ref())
  {
    new_mod->saveToDB();
  }

  if (0 != new_mod->ref())
  {
    if (_owner != nullptr && new_mod->oref() == 0 )
    {
      new_mod->set_oref(_owner->ref());
      new_mod->set_otable(_owner->table());
    }
    _complex_mod->augument(*new_mod);
    _applied_mods.push_back(TimedCreatureModificator(new_mod, new_mod->effect_time()));
  }
  else
  {
    throw error::no_ref("Do managera nie można dodać modyfikatora nei zapisanego w bazie!");
  }
}

bool CreatureModificatorManager::remove(dbRef mod_to_remove)
{
  for (auto i = _applied_mods.begin(); i != _applied_mods.end(); ++i)
  {
    if ( i->modificator->ref() == mod_to_remove )
    {
      i->modificator->set_oref(0);
      i->modificator->set_otable("");
      _complex_mod->remove_augument( *(i->modificator) );
      _applied_mods.erase(i);
      return true;
    }
  }

  return false;
}

std::vector<std::weak_ptr<CreatureModificator> > CreatureModificatorManager::getAll()
{
  vector<weak_ptr<CreatureModificator> > mods;
  for (auto i = _applied_mods.begin(); i != _applied_mods.end(); ++i)
  {
    mods.push_back(i->modificator);
  }

  return mods;
}

void CreatureModificatorManager::tick_time(Minute tick)
{
  for (auto i = _applied_mods.begin(); i != _applied_mods.end();)
  {
    TimedCreatureModificator& mod = (*i);
    bool erased = false;

    //odejmij czas
    if ( mod.time > static_cast<int>(tick)  )
    {
      mod.time -= tick;
    }
    //usun efekt jezeli do usuniecia
    else if ( mod.time != -1 )
    {
      erased = remove(mod.modificator->ref());
    }

    if (!erased) ++i;
  }
}










