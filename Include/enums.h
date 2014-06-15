#ifndef ENUMS_H
#define ENUMS_H

#include "inc.h"
//===
/* Słownik dbRef
 * przydatny przy tworzeniu obiektów
 * przez metody fabryczne, które
 * wymagają podania ref obiektu do stworzenia
 */
namespace refDict
{
  enum class Item
  {
    Miedziak = 121
  };
}
//~~~

//===
enum class WeaponSkill
{
  Null = 0,
  Miecze = 1,
  Topory = 2,
  Cepy = 3,
  Wrecz = 4,
  Luki = 5,
  Wlocznie = 6,
  Noze = 7,
  Proce = 8,
  Kije = 9,
  Improwizacja = 10,
  End = 11
};
//~~~

//===
enum class ItemCondition
{
  Null = 0,
  Damaged = 1,
  Good = 2,
  End = 3
};
//~~~

//===
enum class BodyPartType
{
  Null = 0,
  Head = 1,
  Chest = 2,
  LeftHand = 3,
  RightHand = 4,
  LeftLeg = 5,
  RightLeg = 6,
  RightFinger = 7,
  LeftFinger = 8,
  Neck = 9,
  End = 8
};
//~~~

//===
enum class ItemContainerPrototype
{
  Null = 0,
  Inventory = 1,
  End = 2
};
//~~~

//===
enum class ItemPrototype
{
  Null = 0,
  Nozyk = 1,
  Noz = 2,
  Sztylet_typowy = 3,
  //TODO (...)
  End = 2
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
  Shield = 7,
  LocationObject = 8,
  End = 9
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
