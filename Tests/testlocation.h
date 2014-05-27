#ifndef TESTLOCATION_H
#define TESTLOCATION_H
#include <QtTest/QtTest>

class TestLocation : public QObject
{
  Q_OBJECT
public:
  TestLocation();
private slots:
   void LocCreation();
};


#endif // TESTLOCATION_H
