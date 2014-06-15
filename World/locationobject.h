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
};

#endif // LOCATIONOBJECT_H
