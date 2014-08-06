#ifndef CREATURESTATS_H
#define CREATURESTATS_H


#include "Include/func.h"


class CreatureStats
{
public:
  int attribute(Attribute atr) const;
  int skill(Skill skill) const;

  void setAttribute(Attribute atr, int val);
  void modifyAttribute(Attribute atr, int mod);
  void setSkill(Skill skill, int val);
  void modifySkill(Skill skill, int mod);

  void str2attributes(std::string atr_str);
  void str2skills(std::string skill_str);
  std::string attributes2str() const;
  std::string skills2str() const;
  void augument(const CreatureStats& stats);
  void removeAugument(const CreatureStats& stats);

private:
  std::map<Attribute, int> _attributes;
  std::map<Skill, int> _skills;

};

#endif // CREATURESTATS_H
