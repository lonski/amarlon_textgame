#ifndef ENUMS_H
#define ENUMS_H

#include "inc.h"

//===
enum class BodyPart
{
  Null = 0,
  Head = 1,
  Chest = 2,
  Waist = 3,
  Hands = 4,
  Feet = 5,
  Neck = 6,
  Finger = 7,
  End = 8
};

//===
enum class ItemSizeClass
{
  Null       =  0,
  BardzoMaly =  1,
  Maly       =  2,
  Nieduzy    =  3,
  Spory      =  4,
  Duzy       =  5,
  BardzoDuzy =  6,
  Ogromny    =  7,
  End        =  8
};
/*
1.Bardzo mały
  - Orientacyjna objętość: 1^3[cm3] - 5^3[cm3]
  - Przykłady: mały kamyk, kostka do gry, pierścień
  - Uwagi: raczej brak broni czy pancerza o tym rozmiarze
2.Mały
  - Orientacyjna objętość: 5^3[cm3] - 10^3[cm3]
  - Przykłady: nóż, szklanka, skarpetka
  - Uwagi: raczej brak pancerza w tym rozmiarze
3.Nieduży
  - Orientacyjna objętość: 10^3[cm3] - 20^2[cm3]
  - Przykłady: kwiatek doniczkowy, książka, hełm, którki miecz
4.Spory
  - Orentacyjna objętość: 20^3[cm3] - 40^3[cm3]
  - Przykłady: plecak, mała tarcza, długi miecz, poduszka, kaftan, kolczuga
5.Duży
  - Orientacyjna objętość: 40^3[cm3] - 80^3[cm3]
  - Przykłady: zbroja płytowa, gitara, krzesło, halabarda
6.Bardzo duży
  - Orientacyjna objętość: 80^3[cm3]++
  - Przykłady: człowiek, szafka, beczka
  - Uwagi: jeżeli już możliwy do podniesienia, to zajmujący cały kontener
7. Ogromny
    Item którego nie da się podnieść, czyli np drzewo czy piec
    */
//~~~

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
