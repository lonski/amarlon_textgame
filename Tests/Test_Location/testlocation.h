#ifndef TESTLOCATION_H
#define TESTLOCATION_H
#include "testsuite.h"

#include "World/location.h"
#include <QtTest/QtTest>

class TestLocation : public QTestSuite
{
  Q_OBJECT
public:
  TestLocation();
private slots:
   void LocCreation();
   void LoadingData();
   void LocWalkWithinRange();
   void SaveData();
};

#endif // TESTLOCATION_H

