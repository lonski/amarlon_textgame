#ifndef CREATUREMODIFICATOR_H
#define CREATUREMODIFICATOR_H


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
  const static dbTable tableName;
  virtual dbTable table() const { return tableName; }
  dbTable otable() const {return _otable; }
  dbRef oref() const { return _oref; }
  void setOTable(dbTable otable) { _otable = otable; set_modified();}
  void setORef(dbRef oref) { _oref = oref; set_modified(); }

  //creation
  CreatureModificator(dbTable otable = "", dbRef oref = 0);
  CreatureModificator(dbRef ref);
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
  void setName(std::string name) { _name = name; set_modified(); }
  void set_effect_time(int effect_time) { _effect_time = effect_time; set_modified(); }
  void set_global_test_level_mod(int test_level_mod) { _global_test_level_mod = test_level_mod; set_modified(); }

  //operations
  virtual void load(MapRow *data_source = nullptr);
  virtual void saveToDB();
  void augument(const CreatureModificator& mod);
  void remove_augument(const CreatureModificator& mod);
};

#endif // CREATUREMODIFICATOR_H
