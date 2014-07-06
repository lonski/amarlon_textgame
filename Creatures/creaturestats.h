#ifndef CREATURESTATS_H
#define CREATURESTATS_H

#include "../Include/inc.h"
#include "../Include/func.h"
#include "../Include/enums.h"

class CreatureStats
{
private:
  std::map<Attribute, int> _attributes;
  std::map<Skill, int> _skills;
public:
  //access data
  int attribute(Attribute atr) const { return ( _attributes.count(atr) ? _attributes.at(atr) : 0 ); }
  int skill(Skill skill) const { return ( _skills.count(skill) ? _skills.at(skill) : 0 ); }

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
  void augument(const CreatureStats& stats);
  void remove_augument(const CreatureStats& stats);
};

#endif // CREATURESTATS_H
