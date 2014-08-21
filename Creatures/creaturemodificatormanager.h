#ifndef CREATUREMODIFICATORMANAGER_H
#define CREATUREMODIFICATORMANAGER_H

#include "creaturemodificator.h"

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
  std::vector<std::weak_ptr<CreatureModificator> > getAll();
  virtual void tick_time(Minute tick);
  void save();
};
#endif // CREATUREMODIFICATORMANAGER_H
