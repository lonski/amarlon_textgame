#ifndef LOCATIONOBJECT_H
#define LOCATIONOBJECT_H

#include "../Equipment/item.h"

class LocationObject;
typedef std::shared_ptr<LocationObject> LocationObjectPtr;

class LocationObject : public Item
{
public:
  virtual ~LocationObject() {}

  inline static LocationObject* forge(ItemPrototype proto)
  {
    return dynamic_cast<LocationObject*>(Item::prototypes().clone(proto).release());
  }

private:
  friend class Item;
  LocationObject(dbRef ref, bool temporary = false);

};

#endif // LOCATIONOBJECT_H
