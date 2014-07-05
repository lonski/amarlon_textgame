#ifndef BODYPART_H
#define BODYPART_H

#include "../Include/inc.h"
#include "../Include/comobj.h"
#include "../Include/func.h"
#include "../Include/enums.h"
#include "../Equipment/item.h"
#include "../Equipment/armor.h"

class BodyPart
{
private:
  BodyRegion _region;
  BodySide _side;
  BodyPartType _type;
  DamageLevel _damage;
  Damage _armor;
  std::map<ItemType, std::shared_ptr<Item> > _equipped;

  void calc_armor();
public:
  //creation
  BodyPart();
  BodyPart(BodyPartType type, BodyRegion region, BodySide side);
  BodyPart(std::string str, std::vector<std::shared_ptr<Item> > &eq_items);
  ~BodyPart() {}

  //opeartions
  std::string toStr();
  bool fromStr(std::string str, std::vector<std::shared_ptr<Item> > &eq_items);
  void equip(std::shared_ptr<Item> item);
  std::shared_ptr<Item> unequip(ItemType itype);
  std::vector<std::shared_ptr<Item> > unequip();
  bool accept(ItemType itype);

  //access data
  BodyRegion region() const { return _region; }
  BodySide side() const { return _side; }
  BodyPartType type() const { return _type; }
  DamageLevel damage() const { return _damage; }
  Damage armor() const { return _armor; }
  std::weak_ptr<Item> equipped(ItemType itype);
  std::vector<std::weak_ptr<Item> > equipped();

  //set data
  void set_region(BodyRegion region) { _region = region; }
  void set_side(BodySide side) { _side = side; }
  void set_type(BodyPartType type) { _type = type; }
  void set_damage(DamageLevel damage) { _damage = damage; }

};

#endif // BODYPART_H
