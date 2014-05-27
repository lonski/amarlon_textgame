#include "location.h"

using namespace std;
using namespace soci;

//===LOCATION
unsigned int Location::_draw_range = 5;

Location::Location(Ref ref)
: DBObject(ref), _loaded(false), _drawn(false)
{
  for (auto i = Directions::Null; i != Directions::End; ++i)
    _neighbours[static_cast<Directions>(i)] = nullptr;  
}

std::unique_ptr<Location> Location::create(LocTypes loc_type, Ref ref)
{
  Location *loc_ptr = nullptr;

  switch(loc_type)
  {
    case LocTypes::Ordinary : loc_ptr = new OrdinaryLocation(ref);
    case LocTypes::Draw     : loc_ptr = new DrawLocation(ref);
    default : throw creation_error("Nieprawidłowy typ lokacji.");
  }

  return unique_ptr<Location>(loc_ptr);
}

void Location::loc_walk(WalkVector dir_vector, void (Location::*Fun)())
{
}

void Location::create_neighbours()
{
  MapTable neighbours;
  MapQuery("SELECT * FROM v_loc_neighbours WHERE location="+toString(ref()), neighbours);

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
      //tylko stworzenie, bo jezeli odpalic tutaj load(), to załaduje rekurencyjnie cały świat
      _neighbours[dir] = shared_ptr<Location>( Location::create(loc_type, nb_loc).release() );
    }

  }

}

//===~~~

//===ORDINARY LOCATION
void OrdinaryLocation::load()
{
  if ( !loaded() && ref() > 0 ){
    try
    {
      MapRow loc_data = MapQuery( "SELECT * FROM locations WHERE ref="+toString(ref()) );
      if (loc_data.size() > 0)
      {
        setName( CheckField<string>(loc_data["NAME"]) );
        setDestript( CheckField<string>(loc_data["DESCRIPTION"]) );

        create_neighbours();
      }
    }catch(soci_error &e)
    {
      MsgError(e.what());
      qDebug() << __sql.get_last_query().c_str();
    }
  }
}

void OrdinaryLocation::draw()
{
}
//===~~~

//===DRAW LOCATION
void DrawLocation::load()
{
}

void DrawLocation::draw()
{
}
//===~~~
