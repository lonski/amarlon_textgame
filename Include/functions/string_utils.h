#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <sstream>
#include <vector>
#include <QColor>

namespace fun
{

std::string toLower(std::string str);
std::vector<std::string> explode(std::string str, char ch = ' ');
QColor Str2Color(std::string rgb_color);

template<typename T> T CheckValue(const std::string &field_val, T* = 0);
template<> std::string CheckValue<std::string>(const std::string &field_val, std::string*);
template<typename T> T CheckValueCast(const std::string &field_val);

//============
template<typename T>
static std::string toStr(const T& t)
{
  std::ostringstream os;
  os << t;
  return os.str();
}

template<typename T>
static T fromStr(const std::string& s)
{
  std::istringstream is(s);
  T t;
  is >> t;
  return t;
}

template<typename T>
static std::string toStrPrec(const T& t, const int prec=2)
{
  std::ostringstream ss;
  ss << std::fixed;
  ss.precision(prec);
  ss << t;
  return ss.str();
}

template<typename T>
inline T CheckValue(const std::string &field_val, T*)
{
  if (field_val != "NULL" && field_val != "")
    return fromStr<T>(field_val);
  return fromStr<T>("0");
}

template<>
inline std::string CheckValue(const std::string &field_val, std::string*)
{
  if (field_val != "NULL" && field_val != "")
    return field_val;
  return "";
}

template<typename T>
inline T CheckValueCast(const std::string &field_val)
{
  if (field_val != "NULL" && field_val != "")
    return static_cast<T>(fromStr<int>(field_val));
  return static_cast<T>(0);
}


}

#endif // STRING_UTILS_H
