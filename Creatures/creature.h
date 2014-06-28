#ifndef CREATURE_H
#define CREATURE_H

#include "../Include/inc.h"
#include "../Include/comobj.h"
#include "../Include/func.h"
#include "../Include/enums.h"
#include "../Equipment/item.h"
#include "creaturestats.h"
#include "creaturemodificator.h"
#include "bodypart.h"

class Creature : public DBObject, public Prototypable<Creature, CreaturePrototype>
{
public:
  typedef std::vector<std::shared_ptr<BodyPart> > Body;
private:
  //data
  std::string _name;
  std::string _descript;
  Sex _sex;

  CreatureStats _stats;
  Body _body;
  Item::Inventory _inventory;
  CreatureModificatorManager _mods;

  DamageLevel _total_damage;
  void calc_total_damage();
protected:
  //creation
  Creature(dbRef ref, bool temp = false);

public:
  //parameters
  const static dbTable table_name;
  virtual dbTable table() const { return table_name; }

  //creation
  static std::unique_ptr<Creature> create(dbRef ref, bool prototype = false, bool temp = false);
  std::unique_ptr<Creature> clone();
  virtual ~Creature() = 0;

  //operations
  virtual void load();

  //data access
  std::string name() const { return _name; }
  std::string descript() const { return _descript; }
  Sex sex() const { return _sex; }
  DamageLevel total_damage() const { return _total_damage; }

  CreatureStats& stats() { return _stats; }

  //data set
  void set_name(std::string name);
  void set_descript(std::string descript);
  void set_sex(Sex sex);

  //stats access
  int get_attribute(Attribute atr) const { return _stats.get_attribute(atr) + _mods.get_complex_mod().creature_stats().get_attribute(atr); }
  int get_skill(Skill skill) const { return _stats.get_skill(skill) + _mods.get_complex_mod().creature_stats().get_skill(skill); }

  //stats set
  void set_attribute(Attribute atr, int val);
  void mod_attribute(Attribute atr, int mod);
  void set_skill(Skill skill, int val);
  void mod_skill(Skill skill, int mod);


};

#endif // CREATURE_H
