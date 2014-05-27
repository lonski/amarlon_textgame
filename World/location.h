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
      case Directions::North: return 0;
      case Directions::South: return 1;
      case Directions::East: return 2;
      case Directions::West: return 3;
      default : return -1;
    }
  }

public:
  WalkVector(short north = 0, short south = 0, short east = 0, short west = 0 )
  : _vals{north, south, east, west}
  {}

  void inc(Directions dir, short val = 1) { _vals[indexof(dir)] += val; }
  void dec(Directions dir, short val = 1) { _vals[indexof(dir)] -= val; }
  short operator[](Directions dir) { return _vals[indexof(dir)]; }

};
//===~~

//===Location - abstract base class
class Location : public DBObject{
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
  std::map<Directions, std::shared_ptr<Location> > _neighbours;

protected:
  Location(Ref ref);
  virtual void loc_walk(WalkVector dir_vector, void (Location::*Fun)() );
  virtual void load() = 0;
  virtual void draw() = 0;
  virtual void set_not_drawn() { _drawn = false; }
  virtual void set_not_loaded() { _loaded = false; }

  //data
  void setName(std::string name) { _name = name; }
  void setDestript(std::string dsc) { _descript = dsc; }

  //establish neighbour connections
  virtual void create_neighbours();

public:  
  static std::unique_ptr<Location> create(LocTypes loc_type, Ref ref);
  virtual std::weak_ptr<Location> connection(Directions dir) { return _neighbours[dir]; }

  virtual bool loaded() const { return _loaded; }
  virtual bool enterable() const { return true; }
  virtual unsigned int draw_range() const { return _draw_range; }
  virtual void setDrawRange(unsigned int range) { _draw_range = range; }

  virtual std::string name() const { return _name; }
  virtual std::string descript() const { return _descript; }

  virtual ~Location() {}

};
//===~~~

//==DrawLocation -> Location used only for drawing
class DrawLocation : public Location {
protected:
  virtual void load();
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
  virtual void load();
  virtual void draw();

public:
  OrdinaryLocation(Ref ref): Location(ref) {}
  std::list<std::weak_ptr<Creature> > creatures();
  std::list<std::weak_ptr<Item> > objects();

};
//===~~~

#endif // LOCATION_H
