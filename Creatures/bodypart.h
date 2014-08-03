#ifndef BODYPART_H
#define BODYPART_H

#include "Include/inc.h"
#include "Include/comobj.h"
#include "Include/func.h"
#include "Include/enums.h"
#include "Equipment/item.h"
#include "Equipment/armor.h"

class BodyPart
{
public:
  BodyPart();
  BodyPart(BodyPartType type, BodyRegion region, BodySide side);
  BodyPart(std::string str, std::vector<ItemPtr > &eq_items);
  ~BodyPart() {}

  std::string toStr();
  bool fromStr(std::string str, std::vector<ItemPtr > &eq_items);
  void equip(ItemPtr item);
  ItemPtr unequip(ItemType itype);
  std::vector<ItemPtr > unequip();
  bool accept(ItemType itype);

  BodyRegion region() const;
  BodySide side() const;
  BodyPartType type() const;
  DamageLevel damage() const;
  Damage armor() const;
  std::weak_ptr<Item> equipped(ItemType itype);
  std::vector<std::weak_ptr<Item> > equipped();

  void setRegion(BodyRegion region);
  void setSide(BodySide side);
  void setType(BodyPartType type);
  void setDamage(DamageLevel damage);

private:
  BodyRegion _region;
  BodySide _side;
  BodyPartType _type;
  DamageLevel _damage;
  Damage _armor;
  std::map<ItemType, ItemPtr > _equipped;

  void calc_armor();

};

#endif // BODYPART_H
