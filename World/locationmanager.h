#ifndef LOCATIONMANAGER_H
#define LOCATIONMANAGER_H

#include <map>
#include <string>

class Location;
typedef unsigned int LocationRef;

class LocationManager
{
public:  
  LocationManager();
  ~LocationManager();

  void add(Location *loc);
  void remove(LocationRef ref);
  void purge();
  std::string getPrintableContent();

private:
  std::map<LocationRef, Location*> _locations;

};

#endif // LOCATIONMANAGER_H
