#include "location.h"

using namespace std;
using namespace soci;
using namespace fun;

//===LOCATION
unsigned int Location::_drawRange = 5;
const dbTable Location::table_name = "locations";
Location::LocationManager Location::Manager;

Location::Location(dbRef ref)
: DBObject(ref), _drawn(false)
{
  //initalize neighbours
  for (auto i = Direction::Null; i != Direction::End; ++i)
    _neighbours[i] = nullptr;
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

Location *Location::create(dbRef ref, LocType loc_type)
{
  Location *loc_ptr = nullptr;

  if (ref > 0)
  {
    switch(loc_type)
    {
      case LocType::Ordinary : loc_ptr = new OrdinaryLocation(ref); break;
      case LocType::Draw     : loc_ptr = new DrawLocation(ref); break;
      default : throw error::creation_error("Nieprawidłowy typ lokacji - "+toStr(static_cast<int>(loc_type))); break;
    }

    Location::Manager.add(loc_ptr);
  }

  return loc_ptr;
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
  MapQuery("SELECT * FROM v_loc_neighbours WHERE location="+toStr(ref()), neighbours);

  //for each connection
  for (auto n = neighbours.begin(); n != neighbours.end(); ++n)
  {
    //get connection info
    dbRef nb_loc = CheckField<dbRef>( (*n)["NB_LOCATION"] );
    Direction dir = CheckFieldCast<Direction>( (*n)["DIRECTION"] );
    LocType loc_type = CheckFieldCast<LocType>( (*n)["LOC_TYPE"] );

    //if valid and not yet created, then create location
    if (nb_loc != 0 && dir != Direction::Null && _neighbours[dir] == nullptr)
    {
      //only creation, 'cause if we hit load() here, it would have loaded whole world recursively
      _neighbours[dir] = Location::create(nb_loc, loc_type);
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
        setName( CheckField<string>(loc_data["NAME"]) );
        setDestript( CheckField<string>(loc_data["DESCRIPTION"]) );

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
}

//===~~~

//===ORDINARY LOCATION
//===~~~

//===DRAW LOCATION
//===~~~

//===LOCATION MANAGER
void Location::LocationManager::add(Location *loc)
{
  _locations.push_back(loc);
}

void Location::LocationManager::purge()
{
  std::for_each(_locations.begin(), _locations.end(), [](Location *l){ delete l; } );
  _locations.clear();
}

Location::LocationManager::~LocationManager()
{
  purge();  
}
//===~~~
