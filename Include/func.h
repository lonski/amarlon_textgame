#ifndef FUNC_H
#define FUNC_H

#include "inc.h"
#include "db.h"
#include "enums.h"

namespace fun{

  //declarations
  std::string BodyParts2Str(std::vector<BodyPartType>& parts);
  std::vector<BodyPartType> Str2BodyParts(const std::string &str);
  void MapQuery(const std::string &query, MapTable &result);
  MapRow MapQuery(const std::string &query);  
  Profession skill_group(Skill skill);
  std::vector<std::string> explode(std::string str, char ch = ' ');
  QColor Str2Color(std::string rgb_color);
  std::string Enum2Str(BodyPartType bp);
  std::string Enum2Str(BodyRegion bp);
  std::string Enum2Str(BodySide bp);
  std::string Enum2Str(ItemType bp);
  std::string Enum2Str(Attribute e);
  std::string Enum2Str(Skill e);
  std::string Enum2Str(Direction e, bool shortMode);
  std::string toLower(std::string str);

  template<typename T> T CheckField(const std::string &field_val, T* = 0);
  template<> std::string CheckField<std::string>(const std::string &field_val, std::string*);
  template<typename T> T CheckFieldCast(const std::string &field_val);  
  //===~~~

  //===static inlines
  static inline void delay(int amt){
    QTime dieTime= QTime::currentTime().addMSecs(amt);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
  }

  static inline void MsgError(std::string e)
  {
    //QMessageBox::critical(NULL, "Error",e.c_str());
    qDebug() << "ERROR: " << e.c_str();
  }
  //===~~~

  //===templates
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
    ss << fixed;
    ss.precision(prec);
    ss << t;
    return ss.str();
  }

  template<typename T>
  inline T CheckField(const std::string &field_val, T*)
  {
    if (field_val != "NULL" && field_val != "")
      return fromStr<T>(field_val);
    return fromStr<T>("0");
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
      return static_cast<T>(fromStr<int>(field_val));
    return static_cast<T>(0);
  }
  //===~~~

}

#endif // FUNC_H
