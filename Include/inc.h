#ifndef INC_H
#define INC_H

#include <QDebug>
#include <QMessageBox>
#include <sstream>
#include <iostream>
#include <fstream>
#include <memory>
#include <map>
#include <algorithm>
#include <set>
#include <deque>

#include "soci.h"
#include "soci-firebird.h"

#define D(X) qDebug() << #X << "=" << X;

#define _SAVE_TO_DB_ \
  if ( !isTemporary() && ref() != 0 && modified() )\
  {\
    try\
    {\
      save_to_db();\
    }\
    catch(std::exception &e)\
    {\
      qDebug() << "Error saving " << table_name.c_str() << " " << ref() << " : " << e.what();\
    }\
    catch(...)\
    {\
      qDebug() << "Error saving " << table_name.c_str() << " "  << ref() << ".";\
    }\
  }

typedef std::map<std::string,std::string> MapRow;
typedef std::vector<MapRow> MapTable;

typedef double Weight;

//time
typedef unsigned int Minute;
typedef unsigned int Hour;
typedef unsigned int Day;
typedef unsigned int Month;
typedef unsigned int Year;

typedef unsigned int Splot;
typedef unsigned int BravePoints;

#endif // INC_H
