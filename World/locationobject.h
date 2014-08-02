#ifndef LOCATIONOBJECT_H
#define LOCATIONOBJECT_H

#include "../Equipment/item.h"

class LocationObject : public Item
{
private:
  friend class Item;
  LocationObject(dbRef ref, bool temporary = false);
public:
  virtual ~LocationObject() {}

  inline static LocationObject* Forge(ItemPrototype proto)
  {
    return dynamic_cast<LocationObject*>(Item::prototypes().clone(proto).release());
  }
};

#endif // LOCATIONOBJECT_H
