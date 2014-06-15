#ifndef TESTITEMS_H
#define TESTITEMS_H

#include <QtTest/QtTest>
#include "Equipment/item.h"
#include "Equipment/weapon.h"
#include "Equipment/armor.h"
#include "Equipment/food.h"
#include "Equipment/shield.h"
#include "../Include/func.h"

class TestItems : public QObject
{
  Q_OBJECT
public:
  TestItems() {}
private slots:
   void ItemCreation();
   void LoadData();
   void SaveData();
   void TemporaryItem();
   void PrototypeTest();
};

#endif // TESTITEMS_H
