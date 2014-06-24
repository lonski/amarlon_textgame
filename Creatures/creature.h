#ifndef CREATURE_H
#define CREATURE_H

#include "../Include/inc.h"
#include "../Include/comobj.h"
#include "../Include/func.h"
#include "../Include/enums.h"
#include "../Equipment/item.h"
#include "../Equipment/armor.h"

class Creature
{
public:

  class Stats
  {
  private:
    std::map<Attribute, int> _attributes;
    std::map<Skill, int> _skills;
  public:
    //access data
    int get_attribute(Attribute atr) { return ( _attributes.count(atr) ? _attributes.at(atr) : 0 ); }
    int get_skill(Skill skill) { return ( _skills.count(skill) ? _skills.at(skill) : 0 ); }

    //set data
    void set_attribute(Attribute atr, int val) { _attributes[atr] = val; }
    void mod_attribute(Attribute atr, int mod) { _attributes[atr] += mod; }
    void set_skill(Skill skill, int val) { _skills[skill] = val; }
    void mod_skill(Skill skill, int mod) { _skills[skill] += mod; }

    //operations
    void Str2Attributes(std::string atr_str);
    void Str2Skills(std::string skill_str);
    std::string Attributes2Str();
    std::string Skills2Str();
  };

  class BodyPart
  {
  private:
    BodyRegion _region;
    BodyPartType _type;
    DamageLevel _damage;
    Damage _armor;
    std::shared_ptr<Item> _equipped;

    void calc_armor();
  public:
    //creation
    BodyPart();
    BodyPart(std::string str);
    ~BodyPart() {}

    //opeartions
    std::string toStr();
    bool fromStr(std::string str);
    void equip(std::shared_ptr<Item> item);
    std::shared_ptr<Item> unequip();

    //access data
    BodyRegion region() const { return _region; }
    BodyPartType type() const { return _type; }
    DamageLevel damage() const { return _damage; }
    Damage armor() const { return _armor; }
    std::weak_ptr<Item> equipped() const { return _equipped; }

    //set data
    void set_region(BodyRegion region) { _region = region; }
    void set_type(BodyPartType type) { _type = type; }
    void set_damage(DamageLevel damage) { _damage = damage; }

  };

  Creature();
};

#endif // CREATURE_H
