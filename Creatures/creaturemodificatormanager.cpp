#include "creaturemodificatormanager.h"

using namespace std;

CreatureModificatorManager::CreatureModificatorManager(DBObject *owner)
: _complex_mod( shared_ptr<CreatureModificator>(new CreatureModificator) )
, _owner(owner)
{
  if (owner != nullptr)
  {
    _complex_mod->setORef(owner->ref());
    _complex_mod->setOTable(owner->table());
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
    if (_owner != nullptr)
    {
      new_mod->setORef(_owner->ref());
      new_mod->setOTable(_owner->table());
    }
    _complex_mod->augument(*new_mod);
    _applied_mods.push_back(TimedCreatureModificator(new_mod, new_mod->effect_time()));
  }
  else
  {
    throw error::no_ref("Do managera nie można dodać modyfikatora nei zapisanego w bazie!");
    }
}

void CreatureModificatorManager::add(CreatureModificator* new_mod)
{
  add(shared_ptr<CreatureModificator>(new_mod));
}

bool CreatureModificatorManager::remove(dbRef mod_to_remove)
{
  for (auto i = _applied_mods.begin(); i != _applied_mods.end(); ++i)
  {
    if ( i->modificator->ref() == mod_to_remove )
    {
      i->modificator->setORef(0);
      i->modificator->setOTable("");
      _complex_mod->remove_augument( *(i->modificator) );
      _applied_mods.erase(i);
      return true;
    }
  }

  return false;
}

std::vector<CreatureModificator*> CreatureModificatorManager::getAll()
{
  vector<CreatureModificator* > mods;
  for (auto i = _applied_mods.begin(); i != _applied_mods.end(); ++i)
  {
    mods.push_back(i->modificator.get());
  }

  return mods;
}

size_t CreatureModificatorManager::count()
{
  return _applied_mods.size();
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

void CreatureModificatorManager::updateModificatorsOwner()
{
  if (_owner != nullptr)
  {
    for (auto m = _applied_mods.begin(); m != _applied_mods.end(); ++m)
    {
      (*m).modificator->setORef(_owner->ref());
      (*m).modificator->setOTable(_owner->table());
    }
  }
}

void CreatureModificatorManager::save()
{
  for (auto m = _applied_mods.begin(); m != _applied_mods.end(); ++m)
  {
    TimedCreatureModificator mod = *m;
    mod.modificator->set_effect_time(mod.time);
    mod.modificator->saveToDB();
  }
}

void CreatureModificatorManager::setOwner(DBObject *owner)
{
  _owner = owner;
  updateModificatorsOwner();
}

DBObject* CreatureModificatorManager::owner()
{
  return _owner;
}
