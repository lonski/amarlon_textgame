#include "location.h"

using namespace std;
using namespace soci;
using namespace fun;

//===LOCATION
unsigned int Location::_draw_range = 5;
const dbTable Location::table_name = "locations";
Location::LocationManager Location::Manager;

Location::Location(dbRef ref)
: DBObject(ref), _drawn(false)
{
  //initalize neighbours
  for (auto i = Directions::Null; i != Directions::End; ++i)
    _neighbours[i] = nullptr;
}

void Location::set_name(string name)
{
  _name = name;
  save("NAME", _name);
}

void Location::set_destript(string dsc)
{
  _descript = dsc;
  save("DESCRIPTION", _descript);
}

Location *Location::create(dbRef ref, LocTypes loc_type)
{
  Location *loc_ptr = nullptr;

  switch(loc_type)
  {
    case LocTypes::Ordinary : loc_ptr = new OrdinaryLocation(ref); break;
    case LocTypes::Draw     : loc_ptr = new DrawLocation(ref); break;
    default : throw creation_error("Nieprawidłowy typ lokacji - "+toStr(static_cast<int>(loc_type))); break;
  }

  Location::Manager.add(loc_ptr);
  return loc_ptr;
}

void Location::set_connection(Directions dir, Location *loc)
{
  if ( _neighbours[dir] != loc)
  {
    _neighbours[dir] = loc;

    if ( loc != nullptr )
    {
      stringstream s;

      s << "UPDATE OR INSER INTO loc_neighbours(location, direction, nb_location) VALUES("
        << ref() << ", " << static_cast<int>(dir) << ", " << loc->ref() << ")"
        << " MATCHING(location, direction)";

      save(s.str());
    }
  }
}

void Location::loc_walk_within_range(WalkVector dir_vector, void (Location::*Fun)())
{
  (this->*Fun)();
  for (auto dir = Directions::Null; dir != Directions::End; ++dir)
  {
    if (dir_vector[dir])
    {      
      if (connection(dir) != nullptr )
      {
        dir_vector.dec(dir);
        connection(dir)->loc_walk_within_range(dir_vector,Fun);
      }
    }
  }
}

void Location::create_neighbours()
{
  MapTable neighbours;
  MapQuery("SELECT * FROM v_loc_neighbours WHERE location="+toStr(ref()), neighbours);

  //for each connection
  for (auto n = neighbours.begin(); n != neighbours.end(); ++n)
  {
    //get connection info
    dbRef nb_loc = CheckField<dbRef>( (*n)["NB_LOCATION"] );
    Directions dir = CheckFieldCast<Directions>( (*n)["DIRECTION"] );
    LocTypes loc_type = CheckFieldCast<LocTypes>( (*n)["LOC_TYPE"] );

    //if valid and not yet created, then create location
    if (nb_loc != 0 && dir != Directions::Null && _neighbours[dir] == nullptr)
    {
      //only creation, 'cause if we hit load() here, it would have loaded whole world recursively
      _neighbours[dir] = Location::create(nb_loc, loc_type);
    }
  }

  //set connection with existing locations
  for (auto i = _neighbours.begin(); i != _neighbours.end(); ++i)
  {
    if ( i->second != nullptr )
      copy_connections_to_neighbour(i->first);
  }
}

void Location::copy_connections_to_neighbour(Directions dir)
{
  _neighbours[dir]->set_connection(~dir, this );
  switch (dir)
  {
    case Directions::North :
      _neighbours[dir]->set_connection(Directions::West, connection(Directions::Northwest) );
      _neighbours[dir]->set_connection(Directions::Southwest, connection(Directions::West) );
      _neighbours[dir]->set_connection(Directions::East, connection(Directions::Northeast) );
      _neighbours[dir]->set_connection(Directions::Southeast, connection(Directions::East) );
    break;
    case Directions::Northeast :
      _neighbours[dir]->set_connection(Directions::West, connection(Directions::North) );
      _neighbours[dir]->set_connection(Directions::South, connection(Directions::East) );
    break;
    case Directions::East :
      _neighbours[dir]->set_connection(Directions::North, connection(Directions::Northeast) );
      _neighbours[dir]->set_connection(Directions::Northwest, connection(Directions::North) );
      _neighbours[dir]->set_connection(Directions::Southwest, connection(Directions::South) );
      _neighbours[dir]->set_connection(Directions::South, connection(Directions::Southeast) );
    break;
    case Directions::Southeast :
      _neighbours[dir]->set_connection(Directions::West, connection(Directions::South) );
      _neighbours[dir]->set_connection(Directions::North, connection(Directions::East) );
    break;
    case Directions::South :
      _neighbours[dir]->set_connection(Directions::Northeast, connection(Directions::East) );
      _neighbours[dir]->set_connection(Directions::East, connection(Directions::Southeast) );
      _neighbours[dir]->set_connection(Directions::West, connection(Directions::Southwest) );
      _neighbours[dir]->set_connection(Directions::Northwest, connection(Directions::West) );
    break;
    case Directions::Southwest :
      _neighbours[dir]->set_connection(Directions::East, connection(Directions::South) );
      _neighbours[dir]->set_connection(Directions::North, connection(Directions::West) );
    break;
    case Directions::West :
      _neighbours[dir]->set_connection(Directions::South, connection(Directions::Southwest) );
      _neighbours[dir]->set_connection(Directions::Southeast, connection(Directions::South) );
      _neighbours[dir]->set_connection(Directions::Northeast, connection(Directions::North) );
      _neighbours[dir]->set_connection(Directions::North, connection(Directions::Northwest) );
    break;
    case Directions::Northwest :
      _neighbours[dir]->set_connection(Directions::South, connection(Directions::West) );
      _neighbours[dir]->set_connection(Directions::East, connection(Directions::North) );
    break;
    default :;
  }
}

void Location::load()
{
  if ( !loaded() && ref() > 0 ){
    try
    {
      MapRow loc_data = MapQuery( "SELECT * FROM "+table()+" WHERE ref="+toStr(ref()) );
      if (loc_data.size() > 0)
      {
        set_name( CheckField<string>(loc_data["NAME"]) );
        set_destript( CheckField<string>(loc_data["DESCRIPTION"]) );

        create_neighbours();

        set_loaded();
      }
    }
    catch(soci_error &e)
    {
      MsgError(e.what());
      qDebug() << _Database.get_last_query().c_str();
    }
  }
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
