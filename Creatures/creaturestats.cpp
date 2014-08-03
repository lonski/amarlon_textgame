#include "creaturestats.h"

using namespace std;
using namespace fun;

string CreatureStats::attributes2str() const
{
  string result;
  for(int a = (int)Attribute::Null; a != (int)Attribute::End; ++a )
  {
    Attribute atr = static_cast<Attribute>(a);
    result += toStr( attribute(atr) ) + ";";
  }

  return result;
}

string CreatureStats::skills2str() const
{
  string result;
  for(int a = (int)Skill::Null; a != (int)Skill::End; ++a )
  {
    Skill skl = static_cast<Skill>(a);
    result += toStr( skill(skl) ) + ";";
  }

  return result;
}

int CreatureStats::attribute(Attribute atr) const
{
  return ( _attributes.count(atr) ? _attributes.at(atr) : 0 );
}

int CreatureStats::skill(Skill skill) const
{
  return ( _skills.count(skill) ? _skills.at(skill) : 0 );
}

void CreatureStats::setAttribute(Attribute atr, int val)
{
  _attributes[atr] = val;
}

void CreatureStats::modifyAttribute(Attribute atr, int mod)
{
  _attributes[atr] += mod;
}

void CreatureStats::setSkill(Skill skill, int val)
{
  _skills[skill] = val;
}

void CreatureStats::modifySkill(Skill skill, int mod)
{
  _skills[skill] += mod;
}

void CreatureStats::str2attributes(string atr_str)
{
  vector<string> atrs = explode(atr_str,';');
  for (size_t a = 0; a<atrs.size(); ++a)
  {
    Attribute a_nam = static_cast<Attribute>(a);
    int a_val = fromStr<int>(atrs[a]);

    _attributes[a_nam] = a_val;
  }
}

void CreatureStats::str2skills(string skill_str)
{
  vector<string> skills = explode(skill_str,';');
  for (size_t a = 0; a<skills.size(); ++a)
  {
    Skill s_nam = static_cast<Skill>(a);
    int s_val = fromStr<int>(skills[a]);

    _skills[s_nam] = s_val;
  }
}

void CreatureStats::augument(const CreatureStats &stats)
{
  for (int i = (int)Attribute::Null; i != (int)Attribute::End; ++i)
  {    
    Attribute atr = static_cast<Attribute>(i);
    _attributes[atr] += stats.attribute(atr);
  }
  for (int i = (int)Skill::Null; i != (int)Skill::End; ++i)
  {
    Skill skill = static_cast<Skill>(i);
    _skills[skill] += stats.skill(skill);
    }
}

void CreatureStats::removeAugument(const CreatureStats &stats)
{
  for (int i = (int)Attribute::Null; i != (int)Attribute::End; ++i)
  {
    Attribute atr = static_cast<Attribute>(i);
    _attributes[atr] -= stats.attribute(atr);
  }
  for (int i = (int)Skill::Null; i != (int)Skill::End; ++i)
  {
    Skill skill = static_cast<Skill>(i);
    _skills[skill] -= stats.skill(skill);
  }
}
