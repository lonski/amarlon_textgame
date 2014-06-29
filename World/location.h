#ifndef LOCATION_H
#define LOCATION_H

#include "Include/db.h"
#include "Include/enums.h"
#include "Include/func.h"
#include "Include/exceptions.h"
#include "Equipment/item.h"
#include "locationobject.h"

class Creature;
class Item;

//===WalkVector (used in Location to walk within range)
class WalkVector{
private:
  short _vals[4];
  short indexof(Direction dir){
    switch(dir){
      case Direction::North: return 0; break;
      case Direction::South: return 1; break;
      case Direction::East: return 2; break;
      case Direction::West: return 3; break;
      default : return -1; break;
    }
  }

public:
  WalkVector(short north = 0, short south = 0, short east = 0, short west = 0 )
  : _vals{north, south, east, west}
  {
  }
  void inc(Direction dir, short val = 1)
  {
    short i = indexof(dir);
    if (i != -1) _vals[indexof(dir)] += val;
  }
  void dec(Direction dir, short val = 1)
  {
    short i = indexof(dir);
    if (i != -1) _vals[indexof(dir)] -= val;
  }
  short operator[](Direction dir)
  {
    short i = indexof(dir);
    return ( i != -1 ? _vals[i] : 0);
  }
};
//===~~

//===Location - abstract base class
class Location : public DBObject
{
private:
  //parameters
  static unsigned int _draw_range;

  //flags  
  bool _drawn;

  //data
  std::string _name;
  std::string _descript;

  //connections
  std::map<Direction, Location* > _neighbours;

protected:
  //birth and death
  Location(dbRef ref);
  virtual ~Location() = 0;

  //establish neighbour connections
  virtual void create_neighbours();
  virtual void copy_connections_to_neighbour(Direction dir);

public:  
  const static dbTable table_name;

  //Location Manager
  static class LocationManager
  {
  private:
    std::vector<Location*> _locations;
    void add(Location *loc);        
  public:
    friend class Location;
    void purge();        
    ~LocationManager();
  } Manager;

  //creation
  static Location* create(dbRef ref, LocType loc_type = LocType::Ordinary);

  //operations
  virtual void loc_walk_within_range(WalkVector dir_vector, void (Location::*Fun)() );      
  virtual void load();
  virtual void draw();
  virtual void set_not_drawn() { _drawn = false; }

  //access data
  virtual Location* connection(Direction dir) { return _neighbours[dir]; }
  virtual bool drawn() const { return _drawn; }
  virtual bool enterable() const { return true; }
  virtual unsigned int draw_range() const { return _draw_range; }
  virtual dbTable table() const { return table_name; }

  virtual std::string name() const { return _name; }
  virtual std::string descript() const { return _descript; }

  //set data  
  virtual void set_draw_range(unsigned int range) { _draw_range = range; }
  virtual void set_connection(Direction dir, Location* loc);
  void set_name(std::string name);
  void set_destript(std::string dsc);

};
//===~~~

//==DrawLocation -> a location used only for drawing
class DrawLocation : public Location {
protected:

public:
  DrawLocation(dbRef ref) : Location(ref) {}
  virtual bool enterable() const { return false; }
  virtual ~DrawLocation() {}

};
//==~~~

//===OrdinaryLocation -> an enterable location
class OrdinaryLocation : public Location{
private:
  //Creature::Container<MOB>* _mobs;
  //Creature::Container<NPC>* _npcs;
  std::unique_ptr<LocationObjectContainer> _objects;

protected:

public:
  OrdinaryLocation(dbRef ref): Location(ref)
  {
    try
    {
      _objects = LocationObjectContainer::create( LocationObjectContainer::byOwner(table_name, ref) );
    }
    catch(error::creation_error&)
    {
      _objects = std::unique_ptr<LocationObjectContainer>(nullptr);
    }
  }
  std::vector<std::weak_ptr<Creature> > creatures();
  std::vector<std::weak_ptr<Item> > objects();

  virtual ~OrdinaryLocation()
  {
  }
};
//===~~~

#endif // LOCATION_H
