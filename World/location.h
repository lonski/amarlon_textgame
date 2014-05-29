#ifndef LOCATION_H
#define LOCATION_H

#include "Include/common.h"
#include "Include/enums.h"
#include "Include/func.h"
#include "Include/exceptions.h"

class Creature;
class Item;

//===WalkVector
class WalkVector{
private:
  short _vals[4];
  short indexof(Directions dir){
    switch(dir){
      case Directions::North: return 0; break;
      case Directions::South: return 1; break;
      case Directions::East: return 2; break;
      case Directions::West: return 3; break;
      default : return -1; break;
    }
  }

public:
  WalkVector(short north = 0, short south = 0, short east = 0, short west = 0 )
  : _vals{north, south, east, west}
  {
  }
  void inc(Directions dir, short val = 1)
  {
    short i = indexof(dir);
    if (i != -1) _vals[indexof(dir)] += val;
  }
  void dec(Directions dir, short val = 1)
  {
    short i = indexof(dir);
    if (i != -1) _vals[indexof(dir)] -= val;
  }
  short operator[](Directions dir)
  {
    short i = indexof(dir);
    return ( i != -1 ? _vals[i] : 0);
  }
};
//===~~

//===Location - abstract base class
class Location : public DBObject, public std::enable_shared_from_this<Location>{
private:
  //parameters
  static unsigned int _draw_range;

  //flags
  bool _loaded;
  bool _drawn;

  //data
  std::string _name;
  std::string _descript;

  //connections
  std::map<Directions, Location* > _neighbours;

protected:
  Location(Ref ref);

  //data
  void setName(std::string name) { _name = name; }
  void setDestript(std::string dsc) { _descript = dsc; }

  //establish neighbour connections
  virtual void create_neighbours();
  virtual void copy_connections_to_neighbour(Directions dir);

public:  
  static std::unique_ptr<Location> create(Ref ref, LocTypes loc_type = LocTypes::Ordinary);
  virtual Location* connection(Directions dir) { return _neighbours[dir]; }
  virtual void setConnection(Directions dir, Location* loc);

  //operations
  virtual void loc_walk_within_range(WalkVector dir_vector, void (Location::*Fun)() );
  virtual void load();
  virtual void draw();
  virtual void set_not_drawn() { _drawn = false; }
  virtual void set_not_loaded() { _loaded = false; }

  //set data
  virtual bool loaded() const { return _loaded; }
  virtual bool drawn() const { return _drawn; }
  virtual bool enterable() const { return true; }
  virtual unsigned int draw_range() const { return _draw_range; }
  virtual void setDrawRange(unsigned int range) { _draw_range = range; }

  virtual std::string name() const { return _name; }
  virtual std::string descript() const { return _descript; }

  virtual ~Location() = 0;

};
//===~~~

//==DrawLocation -> Location used only for drawing
class DrawLocation : public Location {
protected:
  virtual void draw();

public:
  DrawLocation(Ref ref) : Location(ref) {}
  virtual bool enterable() const { return false; }
  virtual ~DrawLocation() {}

};
//==~~~

//===OrdinaryLocation -> an enterable location
class OrdinaryLocation : public Location{
private:
  //objects in loc
  std::list<std::shared_ptr<Creature> > _creatures;
  std::list<std::shared_ptr<Item> > _objects;

protected:
  virtual void draw();

public:
  OrdinaryLocation(Ref ref): Location(ref) {}
  std::list<std::weak_ptr<Creature> > creatures();
  std::list<std::weak_ptr<Item> > objects();

  virtual ~OrdinaryLocation() {}
};
//===~~~

#endif // LOCATION_H
