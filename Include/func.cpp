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
    stringstream ss;
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
    stringstream ss;
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
  string str(static_cast<int>(BodyPartType::End), '0');

  for_each(parts.begin(), parts.end(),
    [&str](BodyPartType bp)
    {
      str[static_cast<int>(bp)] = '1';
    }
  );

  return str;
}

std::vector<BodyPartType> fun::Str2BodyParts(const string str)
{
  vector<BodyPartType> p;
  for (size_t i = 0; i < str.size(); ++i)
    if (str[i] == '1') p.push_back(static_cast<BodyPartType>(i));

  return p;
}


SkillGroup fun::skill_group(Skill skill)
{
  const int s = static_cast<int>(skill);
  const int group_cnt = 7;

  for (int i = 1; i < group_cnt; ++i)
  {
    int low_bound = i*10;
    int high_bound = low_bound + 10;

    if (s > low_bound && s < high_bound ) return static_cast<SkillGroup>(i);
  }

  return SkillGroup::Null;
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
