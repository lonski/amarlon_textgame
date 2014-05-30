#include "location.h"

using namespace std;
using namespace soci;
using namespace fun;

//===LOCATION
unsigned int Location::_draw_range = 5;
Location::LocationManager Location::Manager;

Location::Location(Ref ref)
: DBObject(ref), _loaded(false), _drawn(false)
{
  //initalize neighbours
  for (auto i = Directions::Null; i != Directions::End; ++i)
    _neighbours[i] = nullptr;
}

void Location::setName(string name)
{
  _name = name;
  save("NAME", _name);
}

void Location::setDestript(string dsc)
{
  _descript = dsc;
  save("DESCRIPTION", _descript);
}

Location *Location::create(Ref ref, LocTypes loc_type)
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

void Location::setConnection(Directions dir, Location *loc)
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
    Ref nb_loc = CheckField<Ref>( (*n)["NB_LOCATION"] );
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
  _neighbours[dir]->setConnection(~dir, this );
  switch (dir)
  {
    case Directions::North :
      _neighbours[dir]->setConnection(Directions::West, connection(Directions::Northwest) );
      _neighbours[dir]->setConnection(Directions::Southwest, connection(Directions::West) );
      _neighbours[dir]->setConnection(Directions::East, connection(Directions::Northeast) );
      _neighbours[dir]->setConnection(Directions::Southeast, connection(Directions::East) );
    break;
    case Directions::Northeast :
      _neighbours[dir]->setConnection(Directions::West, connection(Directions::North) );
      _neighbours[dir]->setConnection(Directions::South, connection(Directions::East) );
    break;
    case Directions::East :
      _neighbours[dir]->setConnection(Directions::North, connection(Directions::Northeast) );
      _neighbours[dir]->setConnection(Directions::Northwest, connection(Directions::North) );
      _neighbours[dir]->setConnection(Directions::Southwest, connection(Directions::South) );
      _neighbours[dir]->setConnection(Directions::South, connection(Directions::Southeast) );
    break;
    case Directions::Southeast :
      _neighbours[dir]->setConnection(Directions::West, connection(Directions::South) );
      _neighbours[dir]->setConnection(Directions::North, connection(Directions::East) );
    break;
    case Directions::South :
      _neighbours[dir]->setConnection(Directions::Northeast, connection(Directions::East) );
      _neighbours[dir]->setConnection(Directions::East, connection(Directions::Southeast) );
      _neighbours[dir]->setConnection(Directions::West, connection(Directions::Southwest) );
      _neighbours[dir]->setConnection(Directions::Northwest, connection(Directions::West) );
    break;
    case Directions::Southwest :
      _neighbours[dir]->setConnection(Directions::East, connection(Directions::South) );
      _neighbours[dir]->setConnection(Directions::North, connection(Directions::West) );
    break;
    case Directions::West :
      _neighbours[dir]->setConnection(Directions::South, connection(Directions::Southwest) );
      _neighbours[dir]->setConnection(Directions::Southeast, connection(Directions::South) );
      _neighbours[dir]->setConnection(Directions::Northeast, connection(Directions::North) );
      _neighbours[dir]->setConnection(Directions::North, connection(Directions::Northwest) );
    break;
    case Directions::Northwest :
      _neighbours[dir]->setConnection(Directions::South, connection(Directions::West) );
      _neighbours[dir]->setConnection(Directions::East, connection(Directions::North) );
    break;
    default :;
  }
}

void Location::load()
{
  if ( !loaded() && ref() > 0 ){
    try
    {
      MapRow loc_data = MapQuery( "SELECT * FROM locations WHERE ref="+toStr(ref()) );
      if (loc_data.size() > 0)
      {
        setName( CheckField<string>(loc_data["NAME"]) );
        setDestript( CheckField<string>(loc_data["DESCRIPTION"]) );

        create_neighbours();

        _loaded = true;
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
  //TDOD
}

void Location::save_to_db()
{
  for (auto i = _save_queries.begin(); i != _save_queries.end(); ++i)
  {
    //send every entry to DB, commit and delete entry
    //if error occurs, only print it in debug and then delete entry
    try
    {
      if (*i != "")
      {
        _Database << *i;
        _Database.commit();
      }
    }
    catch(soci_error &e)
    {
      qDebug() << "###Error saving location " << ref() << ": ";
      qDebug() << e.what();
      qDebug() << (*i).c_str();
    }
  }

  _save_queries.clear();
}

void Location::save(string query)
{
  if ( loaded() ) _save_queries.push_back(query);
}

template<typename T>
void Location::save(string f_name, T f_val)
{
  if ( ref() && loaded() )
  {
    stringstream s;
    s << "UPDATE locations SET " << f_name << "=\'"<<f_val<<"\' WHERE ref="<<ref();
    _save_queries.push_back(s.str());
  }
}

Location::~Location()
{
  try
  {
    save_to_db();
  }
  catch(std::exception &e)
  {
    qDebug() << "Error saving location " << ref() << " : " << e.what();
  }
  catch(...)
  {
    qDebug() << "Error saving location " << ref() << ",";
  }
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
