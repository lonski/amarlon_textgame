#ifndef CREATUREMONITOR_H
#define CREATUREMONITOR_H

#include "creature.h"

#include "typeinfo"

class CreatureMonitor
{
  Creature *_crt;
public:
  CreatureMonitor(Creature *crt);
  CreatureMonitor();
  void set_creature(Creature *crt);  
  void reset(Creature *crt = nullptr);

  std::string print();
  std::string print_bodyparts();
  std::string print_inventory();
  std::string print_mods();
  std::string print_eq();
};

#endif // CREATUREMONITOR_H
