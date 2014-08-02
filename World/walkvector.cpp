#include "walkvector.h"

short WalkVector::indexof(Direction dir)
{
  switch(dir)
  {
  case Direction::North: return 0; break;
  case Direction::South: return 1; break;
  case Direction::East: return 2; break;
  case Direction::West: return 3; break;
  default : return -1; break;
  }
}

WalkVector::WalkVector(short range)
  : _vals{range, range, range, range}
{
}

WalkVector::WalkVector(short north, short south, short east, short west)
  : _vals{north, south, east, west}
{}

void WalkVector::inc(Direction dir, short val)
{
  short i = indexof(dir);
  if (i != -1) _vals[indexof(dir)] += val;
}

void WalkVector::dec(Direction dir, short val)
{
  short i = indexof(dir);
  if (i != -1) _vals[indexof(dir)] -= val;
}

short WalkVector::operator [](Direction dir) const
{
  short i = indexof(dir);
  return ( i != -1 ? _vals[i] : 0);
}
