#include "bodypart.h"

using namespace std;

BodyPart::BodyPart()
  : _region(BodyRegion::Null)
  , _side(BodySide::Null)
  , _type(BodyPartType::Null)
  , _damage(DamageLevel::Null)
  , _armor(Damage())
  , _equipped(std::shared_ptr<Item>(nullptr))
{
}

BodyPart::BodyPart(string str)
{
  fromStr(str);
}

string BodyPart::toStr()
{
  string result;

  result += fun::toStr( (int)_region ) + ",";
  result += fun::toStr( (int)_side ) + ",";
  result += fun::toStr( (int)_type ) + ",";
  result += fun::toStr( (int)_damage ) + ",";

  if (_equipped != nullptr)  result += fun::toStr(_equipped->ref());
  else result += "0";

  return result;
}

bool BodyPart::fromStr(string str)
{
  static const size_t data_fields_cnt = 5;
  bool result = true;

  vector<string> data = fun::explode(str,',');

  if (data_fields_cnt == data.size())
  {
    _region = static_cast<BodyRegion>( fun::fromStr<int>(data[0]) );
    _side = static_cast<BodySide>( fun::fromStr<int>(data[1]) );
    _type = static_cast<BodyPartType>( fun::fromStr<int>(data[2]) );
    _damage = static_cast<DamageLevel>( fun::fromStr<int>(data[3]) );

    dbRef i_ref = fun::fromStr<dbRef>( data[4] );
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

void BodyPart::calc_armor()
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

void BodyPart::equip(std::shared_ptr<Item> item)
{
  _equipped = item;
  calc_armor();
}

std::shared_ptr<Item> BodyPart::unequip()
{
  std::shared_ptr<Item> r = _equipped;
  _equipped = std::shared_ptr<Item>(nullptr);
  calc_armor();
  return r;
}
