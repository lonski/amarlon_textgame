#ifndef CREATUREMODIFICATOR_H
#define CREATUREMODIFICATOR_H

#include "../Include/inc.h"
#include "../Include/exceptions.h"
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

  //parameters
  dbTable _otable;
  dbRef _oref;
public:
  //parameters
  const static dbTable table_name;
  virtual dbTable table() const { return table_name; }
  dbTable otable() const {return _otable; }
  dbRef oref() const { return _oref; }
  void set_otable(dbTable otable) { _otable = otable; set_modified();}
  void set_oref(dbRef oref) { _oref = oref; set_modified();}

  //creation
  CreatureModificator(dbTable otable = "", dbRef oref = 0);
  CreatureModificator(dbRef ref, bool temporary = false);
  ~CreatureModificator();

  //data access  
  CreatureStats& creature_stats() { return _mods; }
  int attribute(Attribute atr) const { return _mods.attribute(atr); }
  int skill(Skill skill) const { return _mods.skill(skill); }
  const CreatureStats& creature_stats() const { return _mods; }
  int global_test_level_mod() const { return _global_test_level_mod; }
  std::string name() const { return _name; }
  int effect_time() const { return _effect_time; }

  //data mod
  void set_name(std::string name) { _name = name; set_modified(); }
  void set_effect_time(int effect_time) { _effect_time = effect_time; set_modified(); }
  void set_global_test_level_mod(int test_level_mod) { _global_test_level_mod = test_level_mod; set_modified(); }

  //operations
  virtual void load(MapRow *data_source = nullptr);
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
  DBObject *_owner;
public:
  CreatureModificatorManager(DBObject *owner = nullptr);
  ~CreatureModificatorManager();
  void add(std::shared_ptr<CreatureModificator> new_mod);
  bool remove(dbRef mod_to_remove);
  std::shared_ptr<CreatureModificator> get_complex_mod() const { return _complex_mod; }
  std::vector<std::weak_ptr<CreatureModificator> > get_all();
  virtual void tick_time(Minute tick);
};

#endif // CREATUREMODIFICATOR_H
