#include "creaturemanager.h"
#include "creature.h"
#include "World/location.h"
#include "Include/func.h"
#include <algorithm>

using namespace std;

CreatureManager::CreatureManager()
{
}

CreatureManager::~CreatureManager()
{
}

void CreatureManager::add(Creature *crt)
{
  _creatures[crt->ref()] = crt;
}

void CreatureManager::remove(CreatureRef crt_ref)
{
  _creatures.erase(crt_ref);
}

std::vector<Creature*> CreatureManager::findByLocation(LocationRef ref)
{
  vector<Creature*> crts;

  for(auto c = _creatures.begin(); c != _creatures.end(); ++c)
  {
    Creature* crt = c->second;
    if (crt->getLocation() != nullptr && crt->getLocation()->ref() == ref)
    {
      crts.push_back(crt);
    }
  }

  return crts;
}

string CreatureManager::getPrintableContent()
{
  string r = ":: Creature Manager ::\n";

  for (auto c = _creatures.begin(); c!=_creatures.end(); ++c)
  {
    string name = c->second->name();
    string ref = fun::toStr(c->first);

    string loc;
    if (c->second->getLocation() != nullptr)
    {
      loc = fun::toStr( c->second->getLocation()->ref() );
    }

    r += "[" + ref + "]" + (loc.empty() ? " " : "[Loc: "+loc+"] ") + name + "\n";
  }

  r += "=====";

  return r;
}

