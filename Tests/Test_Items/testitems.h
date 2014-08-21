#ifndef TESTITEMS_H
#define TESTITEMS_H

#include "testsuite.h"
#include <QtTest/QtTest>
#include "Equipment/item.h"

class TestItems : public QTestSuite
{
  Q_OBJECT
public:
  TestItems() {}
private slots:
   void ItemCreation();
   void LoadData();
   void SaveData();
   //void TemporaryItem();
   void PrototypeTest();
   void ContainerCreation();
   void ContainerInsertionEraseNonStackable();
   void ContainerInsertionEraseStackable();
   void ItemAsAContainer();
   void ItemModManager();
};

#endif // TESTITEMS_H
