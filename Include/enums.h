#ifndef ENUMS_H
#define ENUMS_H

#include "inc.h"

//===
enum class ItemPrototype
{
  Null = 0,
  Sztylet = 1,
  Szyszak = 2,
  Kolczuga = 3,
  End = 4
};
//~~~

enum class ItemType
{
  Null = 0,
  Ordinary = 1,
  Weapon = 2,
  Armor = 3,
  Food = 4,
  Jewelry = 5,
  Tool = 6,
  End = 7
};

//===
enum class LocType
{
  Null     = 0,
  Ordinary = 1,
  Draw     = 2,
  End      = 3
};

//~~~

//===
enum class ObjType
{
  Null = 0,
  Instance = 1,
  Prototype = 2,
  End = 3
};

//~~~

//===
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

Direction operator~(Direction& d);
Direction& operator++( Direction& d );
const char* DirectionStr(Direction &d);



//~~~

#endif // ENUMS_H
