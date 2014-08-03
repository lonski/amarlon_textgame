#include "bodypart.h"

using namespace std;

BodyPart::BodyPart()
  : _region(BodyRegion::Null)
  , _side(BodySide::Null)
  , _type(BodyPartType::Null)
  , _damage(DamageLevel::Null)
  , _armor(Damage())
{
}

BodyPart::BodyPart(BodyPartType type, BodyRegion region, BodySide side)
  : _region(region)
  , _side(side)
  , _type(type)
  , _damage(DamageLevel::Null)
  , _armor(Damage())
{
}

BodyPart::BodyPart(string str, vector<ItemPtr >& eq_items)
{
  fromStr(str, eq_items);
}

string BodyPart::toStr()
{
  string result;

  result += fun::toStr( (int)_region ) + ",";
  result += fun::toStr( (int)_side ) + ",";
  result += fun::toStr( (int)_type ) + ",";
  result += fun::toStr( (int)_damage ) + ",";

  for (int i = (int)ItemType::Null + 1; i != (int)ItemType::End; ++i)
  {
    if (_equipped.count( (ItemType)i ) > 0)
    {
      if (_equipped[ (ItemType)i ] != nullptr)
      {
        result += fun::toStr(_equipped[ (ItemType)i ]->ref()) + ",";
      }
      else
      {
        result += "0,";
      }
    }
    else
    {
      result += "0,";
    }
  }

  return result;
}

bool BodyPart::fromStr(string str, vector<ItemPtr >& eq_items)
{
  static const size_t data_fields_cnt = 4 + (int)ItemType::End - 1;
  bool result = true;

  vector<string> data = fun::explode(str,',');  

  if (data_fields_cnt == data.size())
  {
    int index = 0;
    _region = static_cast<BodyRegion>( fun::fromStr<int>(data[index++]) );
    _side = static_cast<BodySide>( fun::fromStr<int>(data[index++]) );
    _type = static_cast<BodyPartType>( fun::fromStr<int>(data[index++]) );
    _damage = static_cast<DamageLevel>( fun::fromStr<int>(data[index++]) );

    //load equipped items
    for (int i = (int)ItemType::Null + 1; i != (int)ItemType::End; ++i)
    {
      dbRef i_ref = fun::fromStr<dbRef>( data[index++] );
      if (i_ref != 0)
      {
        //poszukaj itema w zalozonych
        bool found = false;
        for (auto eq = eq_items.begin(); eq != eq_items.end(); ++eq)
        {
          //zmnaleziono itema -> equip wskaźnika
          if ( (*eq)->ref() == i_ref && (*eq)->type() == static_cast<ItemType>(i) )
          {
            equip( *eq );
            found = true;
            break;
          }
        }
        //nie znaleziono itema -> stwórz nowy
        if (!found)
        {
          ItemPtr item( move(Item::create(i_ref)) );
          eq_items.push_back(item);
          equip( item );
        }
      }
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
  if (_equipped.count(ItemType::Armor) > 0)
  {
    Armor *arm = dynamic_cast<Armor*>(_equipped[ItemType::Armor].get());
    if (arm != nullptr)
    {
      _armor = arm->damageReduction();
    }
    else
    {
      _armor.clear();
    }
  }
}

void BodyPart::equip(ItemPtr item)
{
  ItemType itype = item->type();  
  _equipped[itype] = item;

  if (itype == ItemType::Armor) calc_armor();
}

ItemPtr BodyPart::unequip(ItemType itype)
{
  ItemPtr r(nullptr);

  if (_equipped.count(itype) > 0)
  {
    r = _equipped[itype];
    _equipped.erase(itype);
    if (itype == ItemType::Armor) calc_armor();
  }

  return r;
}

std::vector<ItemPtr > BodyPart::unequip()
{
  vector<ItemPtr > result;
  for (int i = (int)ItemType::Null + 1; i != (int)ItemType::End; ++i)
  {
    ItemPtr item = unequip(static_cast<ItemType>(i));
    if (item != nullptr) result.push_back(item);
  }

  return result;
}

bool BodyPart::accept(ItemType itype)
{
  /* na boyparcie mogą byc założone różne itemy, ta funkcja określa
     co może byc założone naraz na jednej cześci ciała.

     Na bodyparcie jednocześnie może być założony tylko jeden z itemów: Weapon, Shield, Tool
     oraz dodatkowo może byc założony Armor i Jewelry
  */

  bool result = false;

  if ( _equipped[itype] == nullptr )
  {
    if ( itype == ItemType::Weapon   ||
         itype == ItemType::Shield   ||
         itype == ItemType::Tool     ||
         itype == ItemType::Ordinary ||
         itype == ItemType::Food
       )
    {
      result = _equipped[ItemType::Weapon] == nullptr &&
               _equipped[ItemType::Shield] == nullptr &&
               _equipped[ItemType::Tool] == nullptr &&
               _equipped[ItemType::Ordinary] == nullptr &&
               _equipped[ItemType::Food] == nullptr;
    }
    else if (itype == ItemType::Armor || itype == ItemType::Jewelry)
    {
      result = true;
    }
  }

  return result;
}

std::weak_ptr<Item> BodyPart::equipped(ItemType itype)
{
  weak_ptr<Item> result;

  if (itype != ItemType::Null)
  {
    auto iter = _equipped.find(itype);
    if (iter != _equipped.end())
    {
      result = iter->second;
    }
  }
  else
  {
    for (auto i = _equipped.begin(); i!=_equipped.end(); ++i)
    {
      if (i->second != nullptr)
      {
        result = i->second;
        break;
      }
    }
  }

  return result;
}

std::vector<std::weak_ptr<Item> > BodyPart::equipped()
{
  vector<weak_ptr<Item> > result;
  for (auto i = _equipped.begin(); i != _equipped.end(); ++i)
  {
    result.push_back( i->second );
  }

  return result;
}
