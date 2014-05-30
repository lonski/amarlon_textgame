#ifndef TESTITEMS_H
#define TESTITEMS_H

#include <QtTest/QtTest>
#include "Equipment/item.h"

class TestItems : public QObject
{
  Q_OBJECT
public:
  TestItems() {}
private slots:
   void ItemCreation();
   void LoadData();
   void PrototypeTest();
};

#endif // TESTITEMS_H
