#ifndef ORDINARYITEM_H
#define ORDINARYITEM_H

#include "item.h"

//==Ordinary Item
class OrdinaryItem : public Item
{
private:
  friend class Item;
  OrdinaryItem(dbRef ref);
public:
  virtual ~OrdinaryItem() {}
};
//===
#endif // ORDINARYITEM_H
