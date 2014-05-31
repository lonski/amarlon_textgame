#include "enums.h"

static const char * Directiontrings[] = { "Null", "North", "South", "West", "East",
                                           "Northeast", "Northwest", "Southeast",
                                           "Southwest", "Up", "Down", "End" };

Direction operator~(Direction& d)
{
  Direction result;
  switch(d)
  {
    case Direction::North : result = Direction::South; break;
    case Direction::South : result = Direction::North; break;

    case Direction::West : result = Direction::East; break;
    case Direction::East : result = Direction::West; break;

    case Direction::Northeast : result = Direction::Southwest; break;
    case Direction::Northwest : result = Direction::Southeast; break;

    case Direction::Southeast : result = Direction::Northwest; break;
    case Direction::Southwest : result = Direction::Northeast; break;

    case Direction::Up : result = Direction::Down; break;
    case Direction::Down : result = Direction::Up; break;

    default : result = Direction::Null; break;
  }
  return result;
}

Direction& operator++( Direction& d )
{
  d = static_cast<Direction>( static_cast<int>(d) + 1 );
  return d;
}

const char *Directiontr(Direction &d)
{
  return Directiontrings[static_cast<int>(d)];
}
