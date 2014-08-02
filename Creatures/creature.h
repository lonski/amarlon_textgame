#ifndef CREATURE_H
#define CREATURE_H

#include "../Include/inc.h"
#include "../Include/comobj.h"
#include "../Include/func.h"
#include "../Include/enums.h"
#include "../Equipment/item.h"
#include "../Equipment/weapon.h"
#include "../Equipment/shield.h"
#include "creaturestats.h"
#include "creaturemodificator.h"
#include "bodypart.h"

class CreatureMonitor;
class Location;

class Creature : public DBObject, public Prototypable<Creature, CreaturePrototype>
{
public:
  class Container : public DBObject
  {
  private:
    //data
    dbTable _otable;
    dbRef _oref;

    //creatures
    std::map<dbRef, std::shared_ptr<Creature> > _creatures;
    void Str2Creatures(std::string crts);
    std::string Creatures2Str();

    friend class TestCreature;
  public:
    //creation
    static dbRef byOwner(dbTable otable, dbRef oref);
    Container(dbRef ref);
    Container();
    ~Container();

    //parameters
    const static dbTable table_name;
    virtual dbTable table() const { return table_name; }

    //operations
    virtual void load(MapRow *data_source = nullptr);
    virtual void saveToDB();

    void insert(std::shared_ptr<Creature>& crt);
    std::shared_ptr<Creature> erase(dbRef crt_ref);
    std::shared_ptr<Creature> find(dbRef crt_ref);
    std::vector<std::shared_ptr<Creature> > getAll();

    //owner data
    dbTable otable() const { return _otable; }
    dbRef oref() const { return _oref; }

    void set_otable(dbTable otable);
    void set_oref(dbRef oref);
  };

private:

  class Body
  {
  public:
    typedef std::vector<std::shared_ptr<BodyPart> > BodyParts;
  private:
    BodyParts _parts;
    Item::STLContainer _equipped_items;
    friend class CreatureMonitor;
  public:
    std::vector<std::shared_ptr<BodyPart> > equip(std::shared_ptr<Item> item);
    std::shared_ptr<Item> unequip(dbRef item_ref);
    BodyParts& parts() { return _parts; }
    std::shared_ptr<BodyPart> part(BodyPartType type, BodyRegion region = BodyRegion::Null, BodySide side = BodySide::Null );
    Item::STLContainer& equipped_items() { return _equipped_items; }
    void load(std::string body_str);
    std::string toStr();
  };

  friend class TestCreature;
  friend class CreatureMonitor;
  //data
  std::string _name;
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
protected:
  //creation
  Creature(dbRef ref, bool temp = false);

  //calculations
  virtual void calc_weapons();
  virtual void calc_total_damage();

public:

  //parameters
  const static dbTable table_name;
  virtual dbTable table() const { return table_name; }

  //creation
  static std::unique_ptr<Creature> create(dbRef ref, bool prototype = false, bool temp = false);
  virtual std::unique_ptr<Creature> clone();
  virtual ~Creature() = 0;

  //operations
  virtual void load(MapRow *data_source = nullptr);
  virtual void saveToDB();
  virtual void purge();

  //data access
  std::string name() const { return _name; }
  std::string descript() const { return _descript; }
  Sex sex() const { return _sex; }
  DamageLevel total_damage() const { return _total_damage; }

  CreatureStats& stats() { return _stats; }

  Location* getLocation() const { return _currentLoc; }
  Location* getPrevLoc() const { return _prevLoc; }

  //data set
  void set_name(std::string name);
  void set_descript(std::string descript);
  void set_sex(Sex sex);

  //stats access
  int attribute(Attribute atr) const { return _stats.attribute(atr) + _mods.get_complex_mod()->creature_stats().attribute(atr); }
  int skill(Skill skill) const { return _stats.skill(skill) + _mods.get_complex_mod()->creature_stats().skill(skill); }

  //stats set
  void set_attribute(Attribute atr, int val);
  void mod_attribute(Attribute atr, int mod);
  void set_skill(Skill skill, int val);
  void mod_skill(Skill skill, int mod);

  //body & inventory & mods
  CreatureModificatorManager& mods() { return _mods; }
  Body& body() { set_modified(); return _body; }
  std::vector< AmountedItem > inventory();

  void take(std::shared_ptr<Item> item, int amount = 1);
  AmountedItem drop(dbRef item_ref, int amount = 1);

  void equip(std::shared_ptr<Item> item);
  std::shared_ptr<Item> unequip(dbRef item_ref);

  virtual Weapon* weapon() { return _weapon; }
  virtual Weapon* offhand() { return _offhand; }
  virtual Shield* shield() { return _shield; }

  void setLocation(Location* loc);

};

#endif // CREATURE_H
