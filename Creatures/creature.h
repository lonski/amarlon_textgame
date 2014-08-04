#ifndef CREATURE_H
#define CREATURE_H

#include "Include/inc.h"
#include "Include/comobj.h"
#include "Include/func.h"
#include "Include/enums.h"
#include "Equipment/item.h"
#include "Equipment/weapon.h"
#include "Equipment/shield.h"
#include "creaturestats.h"
#include "creaturemodificator.h"
#include "body.h"
#include "bodypart.h"
#include "creaturemanager.h"

class CreatureMonitor;
class Location;
class Creature;

typedef std::shared_ptr<Creature> CreaturePtr;

class Creature : public DBObject,
                 public Prototypable<Creature, CreaturePrototype>
{
public:
  class Container;

  const static dbTable tableName;
  virtual dbTable table() const { return tableName; }

  static CreatureManager Manager;

  static Creature* create(dbRef ref, bool prototype = false, bool temp = false);
  virtual Creature *clone();
  virtual ~Creature() = 0;

  virtual void load(MapRow *data_source = nullptr);
  virtual void saveToDB();
  virtual void purge();

  std::string name() const;
  std::string locDescript() const;
  std::string descript() const;
  Sex sex() const;
  DamageLevel totalDamage() const;
  int attribute(Attribute atr) const;
  int skill(Skill skill) const;
  CreatureStats& stats();

  Location* getLocation() const;
  Location* getPrevLoc() const;

  CreatureModificatorManager& mods();
  Body& body();
  std::vector< AmountedItem > inventory();

  void take(ItemPtr item, int amount = 1);
  AmountedItem drop(dbRef item_ref, int amount = 1);
  void equip(ItemPtr item);
  ItemPtr unequip(dbRef item_ref);

  virtual Weapon* weapon();
  virtual Weapon* offhand();
  virtual Shield* shield();

  void setName(std::string name);
  void setDescript(std::string descript);
  void setLocDescript(std::string locDescript);
  void setSex(Sex sex);
  void setAttribute(Attribute atr, int val);
  void modifyAttribute(Attribute atr, int mod);
  void setSkill(Skill skill, int val);
  void modifySkill(Skill skill, int mod);
  void setLocation(Location* loc);

protected:
  Creature(dbRef ref, bool temp = false);

  virtual void calcWeapons();
  virtual void calcTotalDamage();

private:
  friend class TestCreature;
  friend class CreatureMonitor;

  std::string _name;
  std::string _locDescript;
  std::string _descript;
  Sex _sex;

  CreatureStats _stats;
  Body _body;
  Item::Inventory _inventory;

  CreatureModificatorManager _mods;

  DamageLevel _total_damage;

  Weapon* _weapon;
  Weapon* _offhand;
  Shield* _shield;

  Location *_currentLoc;
  Location *_prevLoc;

  Item::Inventory& inventoryContainer();

};

#endif // CREATURE_H
