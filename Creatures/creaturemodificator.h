#ifndef CREATUREMODIFICATOR_H
#define CREATUREMODIFICATOR_H

#include "../Include/inc.h"
#include "creaturestats.h"
#include "../Include/db.h"

class CreatureModificator : public DBObject
{
private:
  CreatureStats _mods;
  int _global_test_level_mod;

  std::string _name;
  int _effect_time;
public:
  //parameters
  const static dbTable table_name;

  //creation
  CreatureModificator();
  CreatureModificator(dbRef ref, bool temporary = false);
  ~CreatureModificator();

  //data access
  virtual dbTable table() const { return table_name; }
  CreatureStats& creature_stats() { return _mods; }
  const CreatureStats& creature_stats() const { return _mods; }
  int global_test_level_mod() const { return _global_test_level_mod; }
  std::string name() const { return _name; }
  int effect_time() const { return _effect_time; }

  //data mod
  void set_name(std::string name) { _name = name; }
  void set_effect_time(int effect_time) { _effect_time = effect_time; }
  void set_global_test_level_mod(int test_level_mod) { _global_test_level_mod = test_level_mod; }

  //operations
  virtual void load();
  virtual void save_to_db();
  void augument(const CreatureModificator& mod);
  void remove_augument(const CreatureModificator& mod);
};

#endif // CREATUREMODIFICATOR_H
