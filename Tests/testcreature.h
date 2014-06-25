#ifndef TESTCREATURE_H
#define TESTCREATURE_H

#include <QtTest/QtTest>
#include "../Include/db.h"
#include "../Include/func.h"
#include "../Creatures/creature.h"
#include "../Creatures/creaturestats.h"
#include "../Creatures/bodypart.h"
#include "../Creatures/creaturemodificator.h"

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

  void modificator_creation();
  void modificator_creation_existing();
  void modificator_load();
  void modificator_save();
  void modificator_augument();
};

#endif // TESTCREATURE_H
