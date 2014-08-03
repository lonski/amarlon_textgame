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
#include <QTime>
#include <QApplication>

#include "soci.h"
#include "soci-firebird.h"

#define _DEBUG(X) qDebug() << #X << "=" << X;
#define _STR(X) #X

#define _saveToDB_ \
  if ( !isTemporary() && ref() != 0 && modified() )\
  {\
    try\
    {\
      saveToDB();\
    }\
    catch(std::exception &e)\
    {\
      qDebug() << "Error saving " << tableName.c_str() << " " << ref() << " : " << e.what();\
    }\
    catch(...)\
    {\
      qDebug() << "Error saving " << tableName.c_str() << " "  << ref() << ".";\
    }\
  }

#ifndef TRACE
#define TRACE 1
#endif

#if TRACE
  #define TRACE_POINT() qDebug() << "[TRACE] " << __PRETTY_FUNCTION__ << "-line " << __LINE__;
#else
  #define TRACE_POINT() do {} while(0);
#endif

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
