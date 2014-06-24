#ifndef TESTCREATURE_H
#define TESTCREATURE_H

#include <QtTest/QtTest>
#include "../Include/db.h"
#include "../Include/func.h"
#include "../Creatures/creature.h"
#include "../Creatures/creaturestats.h"
#include "../Creatures/bodypart.h"

class TestCreature : public QObject
{
  Q_OBJECT
public:
  TestCreature() {}
private slots:
  void stats_atr2str();
  void stats_str2atrs();
  void stats_skills2str();
  void stats_str2skills();
  void bodypart_tostr();
  void bodypart_fromstr();
  void bodypart_equip();
  void bodypart_unequip();
  void bodypart_creation_fromstr();
};

#endif // TESTCREATURE_H
