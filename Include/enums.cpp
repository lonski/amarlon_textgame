#include "enums.h"

static const char * DirectionStrings[] = { "Null", "North", "South", "West", "East",
                                           "Northeast", "Northwest", "Southeast",
                                           "Southwest", "Up", "Down", "End" };

Directions operator~(Directions& d)
{
  Directions result;
  switch(d)
  {
    case Directions::North : result = Directions::South; break;
    case Directions::South : result = Directions::North; break;

    case Directions::West : result = Directions::East; break;
    case Directions::East : result = Directions::West; break;

    case Directions::Northeast : result = Directions::Southwest; break;
    case Directions::Northwest : result = Directions::Southeast; break;

    case Directions::Southeast : result = Directions::Northwest; break;
    case Directions::Southwest : result = Directions::Northeast; break;

    case Directions::Up : result = Directions::Down; break;
    case Directions::Down : result = Directions::Up; break;

    default : result = Directions::Null; break;
  }
  return result;
}

Directions& operator++( Directions& d )
{
  d = static_cast<Directions>( static_cast<int>(d) + 1 );
  return d;
}

const char *DirectionStr(Directions &d)
{
  return DirectionStrings[static_cast<int>(d)];
}
