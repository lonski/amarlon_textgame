#ifndef LOCATION_H
#define LOCATION_H

#include "Include/common.h"
#include "Include/enums.h"
#include "Include/func.h"
#include "Include/exceptions.h"

class Creature;
class Item;

//===Location - abstract base class
class Location : public DBObject
{
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

  //save data
  std::list<std::string> _save_queries;

protected:
  //birth and death
  Location(Ref ref);
  virtual ~Location() = 0;

  //establish neighbour connections
  virtual void create_neighbours();
  virtual void copy_connections_to_neighbour(Directions dir);

  //save changes
  template<typename T> void save(std::string f_name, T f_val);
  void save(std::string query);

public:  

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
  static Location* create(Ref ref, LocTypes loc_type = LocTypes::Ordinary);

  //operations
  virtual void loc_walk_within_range(WalkVector dir_vector, void (Location::*Fun)() );    
  void save_to_db();
  virtual void load();
  virtual void draw();
  virtual void set_not_drawn() { _drawn = false; }
  virtual void set_not_loaded() { _loaded = false; }

  //access data
  virtual Location* connection(Directions dir) { return _neighbours[dir]; }
  virtual bool loaded() const { return _loaded; }
  virtual bool drawn() const { return _drawn; }
  virtual bool enterable() const { return true; }
  virtual unsigned int draw_range() const { return _draw_range; }
  virtual void setDrawRange(unsigned int range) { _draw_range = range; }

  virtual std::string name() const { return _name; }
  virtual std::string descript() const { return _descript; }

  //set data
  virtual void setConnection(Directions dir, Location* loc);
  void setName(std::string name);
  void setDestript(std::string dsc);

};
//===~~~

//==DrawLocation -> a location used only for drawing
class DrawLocation : public Location {
protected:

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

public:
  OrdinaryLocation(Ref ref): Location(ref) {}
  std::list<std::weak_ptr<Creature> > creatures();
  std::list<std::weak_ptr<Item> > objects();

  virtual ~OrdinaryLocation() {}
};
//===~~~

#endif // LOCATION_H
