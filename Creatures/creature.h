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

private:

  class Body
  {
  public:
    typedef std::vector<std::shared_ptr<BodyPart> > BodyParts;
  private:
    BodyParts _parts;
    Item::STLContainer _equipped_items;
  public:
    BodyParts& parts() { return _parts; }
    Item::STLContainer& equipped_items() { return _equipped_items; }
    void load(std::string body_str);
    std::string toStr();
  };

  friend class TestCreature;
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
  virtual void save_to_db();
  virtual void purge();

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

  //body & inventory & mods
  CreatureModificatorManager& mods() { return _mods; }
  Body& body() { return _body; }
  void take(std::shared_ptr<Item> item, int amount = 1);
  void drop(dbRef item_ref, int amount = 1);
  void equip(std::shared_ptr<Item> item);
  std::shared_ptr<Item> unequip(dbRef item_ref);

};

#endif // CREATURE_H
