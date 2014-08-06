#ifndef ITEM_H
#define ITEM_H

#include "Include/typedefs/def_measures.h"
#include "Include/db.h"

#include "Include/prototypemanager.h"
#include "Include/exceptions.h"
#include "Creatures/creaturemodificator.h"
#include "Creatures/creaturemodificatormanager.h"

#include "Include/enums/e_itemprototype.h"
#include "Include/enums/e_itemcondition.h"
#include "Include/enums/e_itemtype.h"
#include "Include/enums/e_body.h"

class Item;
typedef std::shared_ptr<Item> ItemPtr;

class Item : public DBObject, public Prototypable<Item, ItemPrototype>
{
public:
  class Container;
  typedef std::shared_ptr<Item::Container > Inventory;
  typedef std::vector<ItemPtr > STLContainer;

  const static dbTable tableName;
  virtual dbTable table() const { return tableName; }

  static Item* create(dbRef ref, bool prototype = false, bool temporary = false);
  static Item* forge(ItemPrototype proto);
  virtual Item* clone();
  virtual ~Item() = 0;

  virtual void load(MapRow *data_source = nullptr);
  virtual void saveToDB();

  Inventory& inventory();
  CreatureModificatorManager& mods();

  ItemType type() const;
  std::string name() const;
  std::string descript() const;
  Weight weight() const;
  int value() const;
  ItemCondition condition() const;
  int durability() const;
  std::vector<BodyPartType> bodyParts() const;
  bool checkBodyPart(BodyPartType bp) const;
  bool isStackable() const;

  void setType(ItemType type);
  void setName(std::string name);
  void setDescript(std::string dsc);
  void setWeight(Weight weight);
  void setValue(int value);
  void setCondition(ItemCondition condition);
  void setDurability(int dura);
  void addBodyPart(BodyPartType body_part);
  void removeBodyPart(BodyPartType body_part);
  void setStackable(bool stackable);

protected:
  Item(dbRef ref, bool temporary = false);

private:
  std::string BodyParts2Str(std::vector<BodyPartType>& parts);
  std::vector<BodyPartType> Str2BodyParts(const std::string &str);

  ItemType _item_type;
  std::string _name;
  std::string _descript;
  Weight _weight;
  int _value;
  ItemCondition _condition;
  int _durability;
  std::vector<BodyPartType> _bodyParts;
  bool _stackable;

  Inventory _inventory;
  CreatureModificatorManager _mods;

  Item& operator=(const Item&) = delete;
  Item(const Item&) = delete;

};

struct AmountedItem
{
  ItemPtr item;
  int amount;

  AmountedItem(ItemPtr i_item = ItemPtr(nullptr), int i_amount = 1)
    : item(i_item), amount(i_amount)
  {}
};

#endif // ITEM_H
