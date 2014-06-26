#ifndef CREATUREMODIFICATOR_H
#define CREATUREMODIFICATOR_H

#include "../Include/inc.h"
#include "creaturestats.h"
#include "../Include/db.h"
#include "../Include/gameclock.h"

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

class CreatureModificatorManager : TimeObserver
{
private:
  struct TimedCreatureModificator
  {
    TimedCreatureModificator(std::shared_ptr<CreatureModificator> m = std::shared_ptr<CreatureModificator>(nullptr), int t=0)
    : modificator(m), time(t)
    {}
    std::shared_ptr<CreatureModificator> modificator;
    int time;
  };

  std::vector<TimedCreatureModificator> _applied_mods;
  std::shared_ptr<CreatureModificator> _complex_mod;
public:
  CreatureModificatorManager();
  void add(std::shared_ptr<CreatureModificator> new_mod);
  void remove(dbRef mod_to_remove);
  const CreatureModificator& get_complex_mod() const { return *_complex_mod; }
  std::vector<std::weak_ptr<CreatureModificator> > get_all();
  virtual void tick_time(Minute tick);
};

#endif // CREATUREMODIFICATOR_H
