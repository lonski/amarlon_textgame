#include "locationmanager.h"
#include "location.h"
#include <algorithm>

LocationManager::~LocationManager()
{
  purge();
}

void LocationManager::add(Location *loc)
{
  _locations.push_back(loc);
}

void LocationManager::purge()
{
  std::for_each(_locations.begin(), _locations.end(), [](Location *l){ delete l; } );
  _locations.clear();
}

