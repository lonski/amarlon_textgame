#ifndef FUNC_H
#define FUNC_H

#include "common.h"

typedef std::map<std::string,std::string> MapRow;
typedef std::vector<MapRow> MapTable;

//declarations
void MapQuery(const std::string &query, MapTable &result);
MapRow MapQuery(const std::string &query);
template<typename T> T CheckField(const std::string &field_val, T* = 0);
template<> std::string CheckField<std::string>(const std::string &field_val, std::string*);
template<typename T> T CheckFieldCast(const std::string &field_val);
//===~~~

//===static inlines
static inline void MsgError(std::string e)
{
  QMessageBox::critical(NULL, "Error",e.c_str());
}
//===~~~

//===templates
template<typename T>
static std::string toString(const T& t)
{
  std::ostringstream os;
  os << t;
  return os.str();
}

template<typename T>
static T fromString(const std::string& s)
{
  std::istringstream is(s);
  T t;
  is >> t;
  return t;
}

template<typename T>
static std::string toStringPrec(const T& t, const int prec=2)
{
  std::ostringstream ss;
  ss << fixed;
  ss.precision(prec);
  ss << t;
  return ss.str();
}

template<typename T>
inline T CheckField(const std::string &field_val, T*)
{
  if (field_val != "NULL" && field_val != "")
    return fromString<T>(field_val);
  return fromString<T>("0");
}

template<>
inline std::string CheckField(const std::string &field_val, std::string*)
{
  if (field_val != "NULL" && field_val != "")
    return field_val;
  return "";
}

template<typename T>
inline T CheckFieldCast(const std::string &field_val)
{
  if (field_val != "NULL" && field_val != "")
    return static_cast<T>(fromString<int>(field_val));
  return static_cast<T>(0);
}
//===~~~

#endif // FUNC_H
