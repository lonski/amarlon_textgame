#ifndef LOCATION_H
#define LOCATION_H

#include "Include/db.h"


#include "Include/exceptions.h"
#include "Equipment/item.h"
#include "walkvector.h"
#include "locationmanager.h"

class Creature;
class Item;

class Location : public DBObject
{
public:
  const static dbTable tableName;
  static LocationManager Manager;

  Location(dbRef ref);
  ~Location();

  void locWalkWithinRange(WalkVector dir_vector, void (Location::*Fun)() );
  void draw();
  void setNotDrawn() { _drawn = false; }
  void load(MapRow *data_source = nullptr);
  void loadNoParam();
  void saveToDB();

  Location* connection(Direction dir) { return _neighbours[dir]; }
  bool drawn() const { return _drawn; }
  bool enterable() const { return true; }
  unsigned int drawRange() const { return _drawRange; }
  dbTable table() const { return tableName; }

  std::string name() const { return _name; }
  std::string descript() const { return _descript; }

  void setDrawRange(unsigned int range) { _drawRange = range; }
  void setConnection(Direction dir, Location* loc);
  void setName(std::string name);
  void setDestript(std::string dsc);

  void insertObject(ItemPtr& obj);
  ItemPtr eraseObject(dbRef obj_ref);
  ItemPtr findObject(dbRef obj_ref);
  std::vector<ItemPtr > getAllObjects();
  std::vector<Creature*> getAllCreatures();

private:
  friend class Debug;
  static unsigned int _drawRange;
  bool _drawn;

  std::string _name;
  std::string _descript;

  std::map<Direction, Location* > _neighbours;
  std::unique_ptr<Item::Container> _objects;

  void loadCreatures();
  void createNeighbours();
  void copyConnectionsToNeighbour(Direction dir);
  void initalizeNeighbours();
  void initalizeLocationObjects(dbRef ref);
  void createObjectsContainer();
};


#endif // LOCATION_H
