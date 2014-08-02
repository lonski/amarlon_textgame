#ifndef LOCATIONMANAGER_H
#define LOCATIONMANAGER_H

#include <vector>

class Location;

class LocationManager
{
private:
  std::vector<Location*> _locations;
  void add(Location *loc);
public:
  friend class Location;
  void purge();
  ~LocationManager();
};

#endif // LOCATIONMANAGER_H
