#include "location.h"
#include "Creatures/creature.h"
#include "Equipment/item_container.h"
#include "Include/functions/messages.h"

using namespace std;
using namespace soci;
using namespace fun;

unsigned int Location::_drawRange = 5;
const dbTable Location::tableName = "locations";
LocationManager Location::Manager;

Location::Location(dbRef ref)
  : DBObject(ref)
  , _drawn(false)
{
  initalizeNeighbours();
  initalizeLocationObjects(ref);
  Location::Manager.add(this);
}

void Location::initalizeNeighbours()
{
  for (auto i = Direction::Null; i != Direction::End; ++i)
    _neighbours[i] = nullptr;
}

void Location::initalizeLocationObjects(dbRef ref)
{
  dbRef objects_ref = Item::Container::byOwner(tableName, ref);

  if (objects_ref != 0)
  {
    _objects.reset(new Item::Container(objects_ref));
  }
  else
  {
    _objects.reset(nullptr);
  }
}

void Location::createObjectsContainer()
{
  if (_objects == nullptr)
  {
    //TODO REFACTOR
    _objects.reset( new Item::Container );
    _objects->setORef(ref());
    _objects->setOTable(table());
  }
}

void Location::setName(string name)
{
  _name = name;
  set_modified();
}

void Location::setDestript(string dsc)
{
  _descript = dsc;
  set_modified();
}

void Location::insertObject(LocationObjectPtr &obj)
{
  if (_objects == nullptr)
  {
    createObjectsContainer();
  }

  ItemPtr item = std::dynamic_pointer_cast<Item>(obj);
  _objects->insert(item, 1);
}

LocationObjectPtr Location::eraseObject(dbRef obj_ref)
{
  if (_objects == nullptr)
  {
    createObjectsContainer();
  }

  return dynamic_pointer_cast<LocationObject>(_objects->erase(obj_ref, 1).item);
}

LocationObjectPtr Location::findObject(dbRef obj_ref)
{
  if (_objects == nullptr)
  {
    createObjectsContainer();
  }

  return dynamic_pointer_cast<LocationObject>(_objects->find(obj_ref).item);
}

std::vector<LocationObjectPtr > Location::getAllObjects()
{
  vector<LocationObjectPtr > r;

  if (_objects == nullptr)
  {
    createObjectsContainer();
  }

  vector<AmountedItem> a_items = _objects->getAll();

  for (auto o = a_items.begin(); o != a_items.end(); ++o)
  {
    r.push_back(dynamic_pointer_cast<LocationObject>(o->item));
  }

  return r;
}

std::vector<Creature *> Location::getAllCreatures()
{
  return Creature::Manager.findByLocation(ref());
}

void Location::setConnection(Direction dir, Location *loc)
{
  if ( _neighbours[dir] != loc)
  {
    _neighbours[dir] = loc;

    if ( loc != nullptr )
    {
      stringstream s;

      s << "UPDATE OR INSERT INTO loc_neighbours(location, direction, nb_location) VALUES("
        << ref() << ", " << static_cast<int>(dir) << ", " << loc->ref() << ")"
        << " MATCHING(location, direction)";

      save(s.str());
      set_modified();
    }
  }
}

void Location::locWalkWithinRange(WalkVector dir_vector, void (Location::*Fun)())
{
  (this->*Fun)();

  for (auto dir = Direction::Null; dir != Direction::End; ++dir)
  {
    if (dir_vector[dir])
    {
      if (connection(dir) != nullptr )
      {
        dir_vector.dec(dir);
        connection(dir)->locWalkWithinRange(dir_vector,Fun);
      }
    }
  }

}

void Location::createNeighbours()
{
  MapTable neighbours;
  MapQuery("SELECT * FROM loc_neighbours WHERE location="+toStr(ref()), neighbours);

  //for each connection
  for (auto n = neighbours.begin(); n != neighbours.end(); ++n)
  {
    //get connection info
    dbRef nb_loc = CheckValue<dbRef>( (*n)["NB_LOCATION"] );
    Direction dir = CheckValueCast<Direction>( (*n)["DIRECTION"] );

    //if valid and not yet created, then create location
    if (nb_loc != 0 && dir != Direction::Null && _neighbours[dir] == nullptr)
    {
      //only creation, 'cause if we hit load() here, it would have loaded whole world recursively
      _neighbours[dir] = new Location(nb_loc);
    }
  }

  //set connection with existing locations
  for (auto i = _neighbours.begin(); i != _neighbours.end(); ++i)
  {
    if ( i->second != nullptr )
      copyConnectionsToNeighbour(i->first);
  }

  set_modified();
}

void Location::copyConnectionsToNeighbour(Direction dir)
{
  _neighbours[dir]->setConnection(~dir, this );
  switch (dir)
  {
  case Direction::North :
    _neighbours[dir]->setConnection(Direction::West, connection(Direction::Northwest) );
    _neighbours[dir]->setConnection(Direction::Southwest, connection(Direction::West) );
    _neighbours[dir]->setConnection(Direction::East, connection(Direction::Northeast) );
    _neighbours[dir]->setConnection(Direction::Southeast, connection(Direction::East) );
    break;
  case Direction::Northeast :
    _neighbours[dir]->setConnection(Direction::West, connection(Direction::North) );
    _neighbours[dir]->setConnection(Direction::South, connection(Direction::East) );
    break;
  case Direction::East :
    _neighbours[dir]->setConnection(Direction::North, connection(Direction::Northeast) );
    _neighbours[dir]->setConnection(Direction::Northwest, connection(Direction::North) );
    _neighbours[dir]->setConnection(Direction::Southwest, connection(Direction::South) );
    _neighbours[dir]->setConnection(Direction::South, connection(Direction::Southeast) );
    break;
  case Direction::Southeast :
    _neighbours[dir]->setConnection(Direction::West, connection(Direction::South) );
    _neighbours[dir]->setConnection(Direction::North, connection(Direction::East) );
    break;
  case Direction::South :
    _neighbours[dir]->setConnection(Direction::Northeast, connection(Direction::East) );
    _neighbours[dir]->setConnection(Direction::East, connection(Direction::Southeast) );
    _neighbours[dir]->setConnection(Direction::West, connection(Direction::Southwest) );
    _neighbours[dir]->setConnection(Direction::Northwest, connection(Direction::West) );
    break;
  case Direction::Southwest :
    _neighbours[dir]->setConnection(Direction::East, connection(Direction::South) );
    _neighbours[dir]->setConnection(Direction::North, connection(Direction::West) );
    break;
  case Direction::West :
    _neighbours[dir]->setConnection(Direction::South, connection(Direction::Southwest) );
    _neighbours[dir]->setConnection(Direction::Southeast, connection(Direction::South) );
    _neighbours[dir]->setConnection(Direction::Northeast, connection(Direction::North) );
    _neighbours[dir]->setConnection(Direction::North, connection(Direction::Northwest) );
    break;
  case Direction::Northwest :
    _neighbours[dir]->setConnection(Direction::South, connection(Direction::West) );
    _neighbours[dir]->setConnection(Direction::East, connection(Direction::North) );
    break;
  default :;
  }

  set_modified();
}

void Location::load(MapRow *data_source)
{
  if ( !loaded() && ref() > 0 )
  {
    try
    {
      MapRow loc_data;
      if (data_source != nullptr)
      {
        loc_data = *data_source;
      }
      else
      {
        loc_data = MapQuery( "SELECT * FROM "+table()+" WHERE ref="+toStr(ref()) );
      }

      if (!loc_data.empty())
      {
        setName( CheckValue<string>(loc_data["NAME"]) );
        setDestript( CheckValue<string>(loc_data["DESCRIPTION"]) );

        loadCreatures();
        createNeighbours();        

        set_loaded();
        set_not_modified();
      }
    }
    catch(soci_error &e)
    {
      MsgError(e.what());
      qDebug() << _Database.get_last_query().c_str();
    }
  }
}

void Location::loadCreatures()
{
  vector<dbRef> creatures(100);
  _Database << "SELECT ref FROM creatures WHERE location=" << ref(), into(creatures);

  for (auto c = creatures.begin(); c != creatures.end(); ++c)
  {
    if (*c > 0)
    {
      Creature::create(*c)->setLocation(this);
    }
  }

}

void Location::saveToDB()
{
  stringstream save_query;

  save_query << "UPDATE " << table() << " SET"
             << "  NAME='" << _name << "'"
             << " ,DESCRIPTION='" << _descript << "'"
             << " WHERE ref=" << ref();

  save(save_query.str());
  DBObject::saveToDB();
}

void Location::loadNoParam()
{
  load();
}

void Location::draw()
{
  //TODO
}

Location::~Location()
{
  Manager.remove(ref());
}


