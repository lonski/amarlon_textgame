#include "locationmanager.h"
#include "location.h"

#include <algorithm>

using namespace std;

LocationManager::LocationManager()
{
}

LocationManager::~LocationManager()
{
  purge();
}

void LocationManager::add(Location *loc)
{
  _locations[loc->ref()] = loc;
}

void LocationManager::remove(LocationRef ref)
{
  _locations.erase(ref);
}

void LocationManager::purge()
{
  for (auto l = _locations.begin(); l!=_locations.end(); ++l)
  {
    delete l->second;
  }
}

string LocationManager::getPrintableContent()
{
  string r = ":: Location Manager ::\n";

  for (auto l = _locations.begin(); l!=_locations.end(); ++l)
  {
    string name = l->second->name();
    string ref = fun::toStr(l->first);
    r += "[" + ref + "] " + name + "\n";
  }

  r += "=====";

  return r;

}

