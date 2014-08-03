#ifndef BODY_H
#define BODY_H

#include "Equipment/item.h"
#include "bodypart.h"

class Body
{
public:
  typedef std::vector<std::shared_ptr<BodyPart> > BodyPartContainer;

  std::vector<std::shared_ptr<BodyPart> > equip(ItemPtr item);
  ItemPtr unequip(dbRef item_ref);
  BodyPartContainer& parts() { return _parts; }
  std::shared_ptr<BodyPart> part(BodyPartType type, BodyRegion region = BodyRegion::Null, BodySide side = BodySide::Null );
  Item::STLContainer& equipped_items() { return _equipped_items; }
  void load(std::string body_str);
  std::string toStr();

private:
  friend class CreatureMonitor;

  BodyPartContainer _parts;
  Item::STLContainer _equipped_items;

};

#endif // BODY_H
