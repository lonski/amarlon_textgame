#ifndef CREATUREMANAGER_H
#define CREATUREMANAGER_H

#include <map>
#include <vector>

class Creature;

typedef unsigned int CreatureRef;
typedef unsigned int LocationRef;

class CreatureManager
{
public:
  CreatureManager();
  ~CreatureManager();

  void add(Creature *crt);
  void remove(CreatureRef crt_ref);
  std::vector<Creature*> findByLocation(LocationRef ref);
  std::string getPrintableContent();

private:
  std::map<CreatureRef, Creature*> _creatures;

};

#endif // CREATUREMANAGER_H
