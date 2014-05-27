#include "location.h"

unsigned int Location::_draw_range = 5;

Location::Location(Ref ref)
: DBObject(ref), _loaded(false), _drawn(false)
{
  for (auto i = Directions::Null; i != Directions::End; ++i)
    _neighbours[static_cast<Directions>(i)] = nullptr;  
}

void Location::loc_walk(WalkVector dir_vector, void (Location::*Fun)())
{
}
