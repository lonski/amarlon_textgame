#include "enum2str.h"
#include "Include/functions/string_utils.h"

using namespace std;
using namespace fun;

string fun::enum2str(ItemType bp)
{
  string s;
  switch(bp)
  {
    case ItemType::Null: s = "Brak (Null)";
         break;
    case ItemType::End: s = "Brak (End)";
         break;
    case ItemType::Armor: s = "Armor";
         break;
    case ItemType::Food: s = "Food";
         break;
    case ItemType::Jewelry: s = "Jewelry";
         break;
    case ItemType::LocationObject: s = "LocationObject";
         break;
    case ItemType::Ordinary: s = "Ordinary";
         break;
    case ItemType::Shield: s = "Shield";
         break;
    case ItemType::Tool: s = "Tool";
         break;
    case ItemType::Weapon: s = "Weapon";
         break;
    default: s = "Uzupełnij Enum2Str";
         break;
  }
  return s;
}

string fun::enum2str(BodyRegion bp)
{
  string s;
  switch(bp)
  {
    case BodyRegion::Null: s = "Brak (Null)";
         break;
    case BodyRegion::End: s = "Brak (End)";
         break;
    case BodyRegion::Dol: s = "Dol";
         break;
    case BodyRegion::Gora: s = "Gora";
         break;
    default: s = "Uzupełnij Enum2Str";
         break;
  }
  return s;
}

string fun::enum2str(Skill e)
{
  string s;
  switch(e)
  {
    case Skill::Null: s = "Brak (Null)";
         break;
    case Skill::End: s = "Brak (End)";
         break;


    default: s = fun::toStr( static_cast<int>(e) );
         break;
  }
  return s;
}

string fun::enum2str(Attribute e)
{
  string s;
  switch(e)
  {
    case Attribute::Null: s = "Brak (Null)";
         break;
    case Attribute::End: s = "Brak (End)";
         break;
    case Attribute::CHR: s = "CHR";
         break;
    case Attribute::DEX: s = "DEX";
         break;
    case Attribute::END: s = "END";
         break;
    case Attribute::IMP: s = "IMP";
         break;
    case Attribute::INT: s = "INT";
         break;
    case Attribute::REF: s = "REF";
         break;
    case Attribute::STR: s = "STR";
         break;
    case Attribute::WLL: s = "WLL";
         break;



    default: s = "Uzupełnij Enum2Str";
         break;
  }
  return s;
}

string fun::enum2str(BodySide bp)
{
  string s;
  switch(bp)
  {
    case BodySide::Null: s = "Brak (Null)";
         break;
    case BodySide::End: s = "Brak (End)";
         break;
    case BodySide::Center: s = "Srodek";
         break;
    case BodySide::Left: s = "Lewa";
         break;
    case BodySide::Right: s = "Prawa";
         break;
    default: s = "Uzupełnij Enum2Str";
         break;
  }
  return s;
}

string fun::enum2str(BodyPartType bp)
{
  string s;
  switch(bp)
  {
    case BodyPartType::Null: s = "Brak (Null)";
         break;
    case BodyPartType::End: s = "Brak (End)";
         break;
    case BodyPartType::Glowa: s = "Glowa";
         break;
    case BodyPartType::Noga: s = "Noga";
         break;
    case BodyPartType::Reka: s = "Reka";
         break;
    case BodyPartType::Tors: s = "Tors";
         break;
    default: s = "Uzupełnij Enum2Str";
         break;
  }
  return s;
}

string fun::enum2str(Direction e, bool shortMode)
{
  string s;
  switch(e)
  {
    case Direction::Null: s = shortMode ? "B" : "Brak";
         break;
    case Direction::End: s = shortMode ? "B" : "Brak";
         break;
    case Direction::North: s = shortMode ? "N" : "Północ";
         break;
    case Direction::South: s = shortMode ? "S" : "Południe";
         break;
    case Direction::East: s = shortMode ? "E" : "Wschód";
         break;
    case Direction::West: s = shortMode ? "W" : "Zachód";
         break;
    case Direction::Northwest: s = shortMode ? "NW" : "Pólnocny zachód";
         break;
    case Direction::Northeast: s = shortMode ? "NE" : "Północny wschód";
         break;
    case Direction::Southeast: s = shortMode ? "SW" : "Południowy wschód";
         break;
    case Direction::Southwest: s = shortMode ? "SE" : "Południowy zachód";
         break;

    default: s = "Uzupełnij Enum2Str";
         break;
  }
  return s;
}
