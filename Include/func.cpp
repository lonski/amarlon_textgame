#include "func.h"

using namespace std;
using namespace soci;
using namespace fun;

void fun::MapQuery(const string &query, MapTable &result){

  result.clear();
  row r;
  soci::statement stmt = (DB::Session().prepare << query, into(r));

  stmt.execute(true);
  if (stmt.got_data()){
    string str;    
    indicator ind;
    size_t cr(-1);

    do{
      cr++;
      map<string, string> map_row;
      for (size_t i=0; i<r.size(); i++){
        const soci::column_properties & props = r.get_properties(i);
        str = "";

        switch(props.get_data_type())
        {
        case soci::dt_string:
            ind = r.get_indicator(i);
            if (ind == i_ok || ind == i_truncated)
              str = r.get<std::string>(i);
            else
              str = "NULL";
            break;
        case soci::dt_double:
            ind = r.get_indicator(i);
            if (ind == i_ok || ind == i_truncated)
              str = toStr(r.get<double>(i));
            else
              str = "NULL";
            break;
        case soci::dt_integer:
            ind = r.get_indicator(i);
            if (ind == i_ok || ind == i_truncated)
              str = toStr(r.get<int>(i));
            else
              str = "NULL";
            break;
        case soci::dt_unsigned_long_long:
            ind = r.get_indicator(i);
            if (ind == i_ok || ind == i_truncated)
              str = toStr(r.get<unsigned long>(i));
            else
              str = "NULL";
            break;
        case soci::dt_long_long:
            ind = r.get_indicator(i);
            if (ind == i_ok || ind == i_truncated)
              str = toStr(r.get<long long>(i));
            else
              str = "NULL";
            break;
        case soci::dt_date:
            //str = Func::toStr(r.get<std::tm>(i));
            //cout << asctime(&when);
            //date = asctime(&when);
            break;
        }

        map_row[props.get_name()] = str;
      }
      result.push_back(map_row);
    }while (stmt.fetch());
  }
  _Database.commit();

}

MapRow fun::MapQuery(const string &query){

  MapRow result;
  row r;
  soci::statement stmt = (_Database.prepare << query, into(r));

  stmt.execute(true);
  if (stmt.got_data()){
    string str;    
    indicator ind;
    size_t cr(-1);

    do{
      cr++;
      for (size_t i=0; i<r.size(); i++){
        const soci::column_properties & props = r.get_properties(i);
        str = "";

        switch(props.get_data_type())
        {
        case soci::dt_string:
            ind = r.get_indicator(i);
            if (ind == i_ok || ind == i_truncated)
              str = r.get<std::string>(i);
            else
              str = "NULL";
            break;
        case soci::dt_double:
            ind = r.get_indicator(i);
            if (ind == i_ok || ind == i_truncated)
              str = toStr(r.get<double>(i));
            else
              str = "NULL";
            break;
        case soci::dt_integer:
            ind = r.get_indicator(i);
            if (ind == i_ok || ind == i_truncated)
              str = toStr(r.get<int>(i));
            else
              str = "NULL";
            break;
        case soci::dt_unsigned_long_long:
            ind = r.get_indicator(i);
            if (ind == i_ok || ind == i_truncated)
              str = toStr(r.get<unsigned long>(i));
            else
              str = "NULL";
            break;
        case soci::dt_long_long:
            ind = r.get_indicator(i);
            if (ind == i_ok || ind == i_truncated)
              str = toStr(r.get<long long>(i));
            else
              str = "NULL";
            break;
        case soci::dt_date:
            //str = Func::toStr(r.get<std::tm>(i));
            //cout << asctime(&when);
            //date = asctime(&when);
            break;
        }

        result[props.get_name()] = str;
      }
    }while (stmt.fetch());
  }
  _Database.commit();
  return result;

}
/*
 * Function converts BodyPartTypes to a 0-1 string, which is saved to db
 */
string fun::BodyParts2Str(vector<BodyPartType> &parts)
{
  string str("");//(static_cast<int>(BodyPartType::End), '0');

  for (auto i = parts.begin(); i != parts.end(); ++i)
  {
    BodyPartType bp = *i;
    str += fun::toStr(static_cast<int>(bp)) + ",";
  }

//  for_each(parts.begin(), parts.end(),
//    [&str](BodyPartType bp)
//    {
//      str[static_cast<int>(bp)] = '1';
//    }
//  );

  return str;
}

std::vector<BodyPartType> fun::Str2BodyParts(const string &str)
{
  vector<BodyPartType> p;

//  for (size_t i = 0; i < str.size(); ++i)
//    if (str[i] == '1') p.push_back(static_cast<BodyPartType>(i));

  vector<string> sv = fun::explode(str, ',');
  for (auto s = sv.begin(); s != sv.end(); ++s)
    p.push_back(static_cast<BodyPartType>( fun::fromStr<int>(*s) ));

  return p;
}


Profession fun::skill_group(Skill skill)
{
  const int s = static_cast<int>(skill);
  const int group_cnt = 7;

  for (int i = 1; i < group_cnt; ++i)
  {
    int low_bound = i*10;
    int high_bound = low_bound + 10;

    if (s > low_bound && s < high_bound ) return static_cast<Profession>(i);
  }

  return Profession::Null;
}


std::vector<string> fun::explode(string str, char ch)
{
  vector<string> result;
  string line;

  for (auto s = str.begin(); s != str.end(); ++s)
  {
    if (*s == ch)
    {
      if (!line.empty())
      {
        result.push_back(line);
        line.clear();
      }
    }
    else
    {
      line += *s;
    }
  }

  if (!line.empty())
  {
    result.push_back(line);
    line.clear();
  }

  return result;
}

string fun::Enum2Str(ItemType bp)
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

string fun::Enum2Str(BodyRegion bp)
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

string fun::Enum2Str(Skill e)
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

string fun::Enum2Str(Attribute e)
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

string fun::Enum2Str(BodySide bp)
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

string fun::Enum2Str(BodyPartType bp)
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


QColor fun::Str2Color(string rgb_color)
{
  QColor kolor;
  vector<string> colors = fun::explode(rgb_color, ';');
  if (colors.size() == 3)
  {
    int r = fun::fromStr<int>(colors[0]);
    int g = fun::fromStr<int>(colors[1]);
    int b = fun::fromStr<int>(colors[2]);
    kolor = QColor(r,g,b);
  }

  return kolor;
}
