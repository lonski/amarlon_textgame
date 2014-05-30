#ifndef ENUMS_H
#define ENUMS_H

#include "inc.h"

//===
enum class ItemPrototypes
{
  Null = 0,
  Dagger = 1,
  End = 2
};
//~~~

enum class ItemTypes
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
enum class LocTypes
{
  Null     = 0,
  Ordinary = 1,
  Draw     = 2,
  End      = 3
};

//~~~

//===
enum class ObjTypes
{
  Null = 0,
  Instance = 1,
  Prototype = 2,
  End = 3
};

//~~~

//===
enum class Directions
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

Directions operator~(Directions& d);
Directions& operator++( Directions& d );
const char* DirectionStr(Directions &d);



//~~~

#endif // ENUMS_H
