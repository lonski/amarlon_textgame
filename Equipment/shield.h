#ifndef SHIELD_H
#define SHIELD_H

#include "item.h"

class Shield;
typedef std::shared_ptr<Shield> ShieldPtr;

class Shield : public Item
{
public:

  virtual ~Shield();

  virtual void load(MapRow *data_source = nullptr);
  virtual void saveToDB();

  int defence() const { return _defence; }

  void setDefence(int defence);

  inline static Shield* forge(ItemPrototype proto)
  {
    return dynamic_cast<Shield*>(Item::prototypes().clone(proto).release());
  }

private:
  friend class Item;
  Shield(dbRef ref,bool temporary = false);

  int _defence;

};

#endif // SHIELD_H
