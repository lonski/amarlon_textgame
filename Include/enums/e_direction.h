#ifndef E_DIRECTION_H
#define E_DIRECTION_H

enum class Direction
{
  Null      = 0,
  North     = 1,
  South     = 2,
  West      = 3,
  East      = 4,
  Northeast = 5,
  Northwest = 6,
  Southeast = 7,
  Southwest = 8,
  Up        = 9,
  Down      = 10,
  End       = 11
};

static const char * Directiontrings[] = { "Null", "North", "South", "West", "East",
                                           "Northeast", "Northwest", "Southeast",
                                           "Southwest", "Up", "Down", "End" };

static inline Direction operator~(Direction& d)
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

static inline Direction& operator++( Direction& d )
{
  d = static_cast<Direction>( static_cast<int>(d) + 1 );
  return d;
}

static inline const char *Directiontr(Direction &d)
{
  return Directiontrings[static_cast<int>(d)];
}

#endif // E_DIRECTION_H
