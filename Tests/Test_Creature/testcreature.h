#ifndef TESTCREATURE_H
#define TESTCREATURE_H

#include "testsuite.h"

#include <QtTest/QtTest>
#include "../Include/db.h"

#include "../Creatures/creature.h"
#include "../Creatures/creaturemonitor.h"
#include "../Creatures/player.h"

class TestCreature : public QTestSuite
{
  Q_OBJECT
public:
  TestCreature() {}
private slots:
  void creature_creation();
  void creature_load_base();
  void creature_save_base();
  void creature_load_save_stats();
  void creature_load_save_body();
  void creature_load_inventory();
  void creature_load_modificators();
  void creature_eq();

//  void npc_load();

  void player_base();
  void player_load();

};

#endif // TESTCREATURE_H
