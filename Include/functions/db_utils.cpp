#include "db_utils.h"
#include "soci.h"
#include "Include/db.h"
#include "Include/functions/string_utils.h"

using namespace std;
using namespace soci;

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
