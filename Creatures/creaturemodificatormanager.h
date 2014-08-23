#ifndef CREATUREMODIFICATORMANAGER_H
#define CREATUREMODIFICATORMANAGER_H

#include "creaturemodificator.h"

class CreatureModificatorManager : TimeObserver
{
public:
  CreatureModificatorManager(DBObject *owner = nullptr);
  ~CreatureModificatorManager();

  void save();
  void setOwner(DBObject *owner);
  DBObject *owner();

  void add(std::shared_ptr<CreatureModificator> new_mod);
  void add(CreatureModificator* new_mod);
  bool remove(dbRef mod_to_remove);
  std::shared_ptr<CreatureModificator> get_complex_mod() const { return _complex_mod; }
  std::vector<CreatureModificator *> getAll();
  size_t count();
  virtual void tick_time(Minute tick);

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

  void updateModificatorsOwner();

};
#endif // CREATUREMODIFICATORMANAGER_H
