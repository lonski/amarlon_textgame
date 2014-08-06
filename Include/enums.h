#ifndef ENUMS_H
#define ENUMS_H


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
  enum class Location
  {
    StartLocation = 1
  };
}
//~~~

enum class HungerLevel
{
//TODO
};

enum class FatigueLevel
{
//TODO
};

enum class CommandID
{
/* Nie zapomnij dodac komendy do Command::create_by_enum! */
  Null = 0,
  Go = 1,
  Take = 2,
  Exit = 3,
  GameMenu = 4,
  Debug = 5,
  End = 6
};

enum class Bless
{
  Null = 0,
  Dziecie_Ognia = 1,
  Dziecie_Wiatru = 2,
  Dziecie_Wody = 3,
  Dziecie_Ziemi = 4,
  Dziecie_Jastrzebia = 5,
  Dziecie_Weza = 6,
  Dziecie_Kota = 7,
  Dziecie_Kruka = 8,
  Dziecie_Niedzwiedzia = 9,
  Dziecie_Wilka = 10,
  Dziecie_Ksiezyca = 11,
  Dziecie_Slonca = 12,
  End = 13
};

enum class Clan
{
  Null = 0,
  Van_Moragh = 1,
  Van_Tarren = 2,
  Van_Bethar = 3,
  Aep_Amra = 4,
  Aep_Tien = 5,
  Aep_Kennal = 6,
  Di_Garren = 7,
  Di_Eise = 8,
  Di_Braugh = 9,
  Mac_Merna = 10,
  Mac_Ferne = 11,
  Mac_Brionna = 12,
  Bal_Dorga = 13,
  Bal_Sethna = 14,
  Bal_Morgh = 15,
  Ar_Kadran = 16,
  Ar_Vesse = 17,
  Ar_Athul = 18,
  O_Rolley = 19,
  O_Branna = 20,
  O_Loner = 21,
  Til_Silla = 22,
  Til_Ollen = 23,
  Til_Meadir = 24,
  End = 25
};

//===
enum class Tribe
{
  Null = 0,
  Tuatha_De_Birde = 1,
  Tuatha_De_Cael = 2,
  Tuatha_De_Edha = 3,
  Tuatha_De_Loch = 4,
  Tuatha_De_Morra = 5,
  Tuatha_De_Raghar = 6,
  Tuatha_De_Sulen = 7,
  Tuatha_De_Talein = 8,
  Mieszaniec = 9,
  End = 10
};
//~~~

//===
enum class CreatureType
{
  Null = 0,
  MOB = 1,
  NPC = 2,
  Player = 3,
  End = 4
};
//~~~

//===
enum class CreaturePrototype
{
  Null = 0,
  BlankMOB = 1,
  Ogr = 2,
  BlankNPC = 3,
  End = 4
};
//~~~

//===
enum class Sex
{
  Null = 0,
  Male = 1,
  Female = 2,
  NotSpecified = 3,
  End = 4
};
//~~~

//===
enum class Season
{
  Null = 0,
  Onn = 1,
  Ur = 2,
  Edad = 3,
  Dair = 4,
  Ailme = 5,
  Iur = 6,
  End = 7
};

//~~~

enum class Profession
{
  Null = 0,
  Wojownik = 1,
  Oszust = 2,
  Lowca = 3,
  Kaplan = 4,
  Bard = 5,
  Brak = 6,
  End = 7
};

enum class Skill
{
  Null = 0,

  //wojownik
  Inicjatywa = 11,
  Kowalstwo = 12,
  Odpornosc = 13,
  Taktyka = 14,
  Tarczownictwo = 15,
  Walka_Miecze = 16,
  Walka_Topory = 17,
  Walka_Cepy = 18,
  Walka_Wrecz = 19,

  //oszust
  Akrobatyka = 21,
  Czujnosc = 22,
  Gadanina = 23,
  Targowanie_sie = 24,
  Ukrywanie_sie = 25,
  Walka_Noze = 26,
  Walka_Proce = 27,
  Wycena = 28,
  Zlodziejstwo = 29,

  //lowca
  Lowiectwo = 31,
  Plywanie = 32,
  Przetrwanie = 33,
  Pulapki = 34,
  Sprawnosc = 35,
  Tropienie = 36,
  Walka_Luki = 37,
  Walka_Wlocznie = 38,
  Wspinaczka = 39,

  //kaplan
  Czytania_Pisanie = 41,
  Empatia = 42,
  Leczenie = 43,
  Rytualy = 44,
  Magia = 45,
  Walka_Kije = 46,
  Wiedza_tajemna = 47,
  Zastraszanie = 48,
  Zielarstwo = 49,

  //bard
  Aktorstwo = 51,
  Autorytet = 52,
  Muzyka = 53,
  Opowiesci = 54,
  Piesni = 55,
  Sztuka = 56,
  Spiew = 57,
  Uwodzenie = 58,
  Walka_improwizowana = 59,

  //wspolne
  Gornictwo = 61,
  Jezdziectwo = 62,
  Mocna_glowa = 63,
  Opieka_nad_zwierzetami = 64,
  Powozenie = 65,
  Rolnictwo = 66,
  Rybactwo = 67,
  Rzemioslo = 68,
  Taniec = 69,

  End = 70
};


enum class Attribute
{
  Null = 0,
  STR  = 1, //Sila
  DEX  = 2, //Zreczność
  END  = 3, //Wytrzymałość
  REF  = 4, //Reflex
  INT  = 5, //Inteligencja
  CHR  = 6, //Charyzma
  IMP  = 7, //Wrażenie
  WLL  = 8, //Wola
  End  = 9
};

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
enum class DamageLevel
{
  Null = 0,
  Brak = 1,
  BardzoLekkie = 2,
  Lekkie = 3,
  Srednie = 4,
  Ciezkie = 5,
  BardzoCiezkie = 6,
  Zabojcze = 7,
  End = 8
};

//~~~

//===
enum class BodySide
{
  Null = 0,
  Left = 1,
  Center = 2,
  Right = 3,
  End = 4
};
//~~~

//===
enum class BodyRegion
{
  Null = 0,
  Gora = 1,
  Dol = 2,
  End = 3
};

//~~~

//===
enum class BodyPartType
{
  Null = 0,
  Glowa = 1,
  Tors = 2,
  Reka = 3,
  Noga = 4,
  End = 5
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
  BlankOrdinary = 4,
  BlankWeapon = 5,
  BlankArmor = 6,
  BlankFood = 7,
  BlankJewelry = 8,
  BlankTool = 9,
  BlankShield = 10,
  BlankLocationObject = 11,
  LekkaZbrojaSkorzana = 12,
  MalaDrewnianaTarcza = 13,
  HelmSkorzany = 14,
  MieczSzeroki = 15,
  //TODO (...)
  End = 16
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
