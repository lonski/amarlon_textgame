#include "creature.h"

using namespace std;
using namespace fun;

Creature::Creature()
{
}

//==========================STATS===============================================
string Creature::Stats::Attributes2Str()
{
  string result;
  for(int a = (int)Attribute::Null; a != (int)Attribute::End; ++a )
  {
    Attribute atr = static_cast<Attribute>(a);
    result += toStr( get_attribute(atr) ) + ";";
  }

  return result;
}

string Creature::Stats::Skills2Str()
{
  string result;
  for(int a = (int)Skill::Null; a != (int)Skill::End; ++a )
  {
    Skill skl = static_cast<Skill>(a);
    result += toStr( get_skill(skl) ) + ";";
  }

  return result;
}

void Creature::Stats::Str2Attributes(string atr_str)
{
  vector<string> atrs = explode(atr_str,';');
  for (size_t a = 0; a<atrs.size(); ++a)
  {
    Attribute a_nam = static_cast<Attribute>(a);
    int a_val = fromStr<int>(atrs[a]);

    _attributes[a_nam] = a_val;
  }
}

void Creature::Stats::Str2Skills(string skill_str)
{
  vector<string> skills = explode(skill_str,';');
  for (size_t a = 0; a<skills.size(); ++a)
  {
    Skill s_nam = static_cast<Skill>(a);
    int s_val = fromStr<int>(skills[a]);

    _skills[s_nam] = s_val;
  }
}
//==================================================================================

//==============================BODY PART===========================================
Creature::BodyPart::BodyPart()
: _region(BodyRegion::Null), _type(BodyPartType::Null), _damage(DamageLevel::Null), _armor(Damage()), _equipped(std::shared_ptr<Item>(nullptr))
{
}

Creature::BodyPart::BodyPart(string str)
{
  fromStr(str);
}

string Creature::BodyPart::toStr()
{
  string result;

  result += fun::toStr( (int)_region ) + ",";
  result += fun::toStr( (int)_type ) + ",";
  result += fun::toStr( (int)_damage ) + ",";

  if (_equipped != nullptr)  result += fun::toStr(_equipped->ref());
  else result += "0";

  return result;
}

bool Creature::BodyPart::fromStr(string str)
{
  static const size_t data_fields_cnt = 4;
  bool result = true;

  vector<string> data = fun::explode(str,',');

  if (data_fields_cnt == data.size())
  {
    _region = static_cast<BodyRegion>( fun::fromStr<int>(data[0]) );
    _type = static_cast<BodyPartType>( fun::fromStr<int>(data[1]) );
    _damage = static_cast<DamageLevel>( fun::fromStr<int>(data[2]) );

    dbRef i_ref = fun::fromStr<dbRef>( data[3] );
    if (0 != i_ref)
    {
      shared_ptr<Item> item( move(Item::create(i_ref)) );
      equip(item);
    }
    else
    {
      _equipped = shared_ptr<Item>(nullptr);
    }
  }
  else
  {
    result = false;
  }

  return result;
}

void Creature::BodyPart::calc_armor()
{
  Armor *arm = dynamic_cast<Armor*>(_equipped.get());
  if (arm != nullptr)
  {
    _armor = arm->damage_reduction();
  }
  else
  {
    _armor.clear();
  }
}

void Creature::BodyPart::equip(std::shared_ptr<Item> item)
{
  _equipped = item;
  calc_armor();
}

std::shared_ptr<Item> Creature::BodyPart::unequip()
{
  std::shared_ptr<Item> r = _equipped;
  _equipped = std::shared_ptr<Item>(nullptr);
  calc_armor();
  return r;
}
//==================================================================================
