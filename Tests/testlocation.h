#ifndef TESTLOCATION_H
#define TESTLOCATION_H

#include "World/location.h"
#include <QtTest/QtTest>

class TestLocation : public QObject
{
  Q_OBJECT
public:
  TestLocation();
private slots:
   void LocCreation();
   void LoadingData();
   void LocWalkWithinRange();
};


#endif // TESTLOCATION_H
