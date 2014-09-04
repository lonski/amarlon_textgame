#ifndef ITEM_H
#define ITEM_H

#include "Include/typedefs/def_measures.h"
#include "Include/db.h"

#include "Include/prototypemanager.h"
#include "Include/exceptions.h"
#include "Creatures/creaturemodificator.h"
#include "Creatures/creaturemodificatormanager.h"
#include "Include/damage.h"

#include "Include/enums/e_itemprototype.h"
#include "Include/enums/e_itemcondition.h"
#include "Include/enums/e_itemtype.h"
#include "Include/enums/e_body.h"
#include "Include/enums/e_weaponskill.h"

class Item;
class DataGateway;
typedef std::shared_ptr<Item> ItemPtr;

class Item : public DBObject, public Prototypable<Item, ItemPrototype>
{
public:  
  class Container;
  typedef std::shared_ptr<Item::Container > Inventory;
  typedef std::vector<ItemPtr > STLContainer;

  static DataGateway* itemsGateway;

  const static dbTable tableName;
  virtual dbTable table() const { return tableName; }

  Item(dbRef ref);
  virtual ~Item();
  static Item* create(dbRef ref, bool prototype = true);
  static Item* forge(ItemPrototype proto);
  virtual Item* clone();

  virtual void load(MapRow* = nullptr);
  virtual void saveToDB();

  std::string getBodyPartsString();
  std::vector<BodyPartType> setBodyParts(const std::string &str);

  Inventory& inventory();
  void setInventory(Item::Container* inv);
  CreatureModificatorManager* mods();

//item specyfic
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

//weapon specific
  WeaponSkill weaponSkill() const { return _wpn_skill; }
  Damage damage() const { return _damage; }
  int defence() const { return _defence; }
  int attack() const { return _attack; }
  int reflex() const { return _reflex; }
  int str_req() const { return _str_req; }
  int range() const { return _range; }

  void setWeaponSkill(WeaponSkill weaponSkill);
  void setDamage(Damage damage);
  void setDefence(int defence);
  void setAttack(int attack);
  void setReflex(int reflex);
  void setStrReq(int val);
  void setRange(int range);

//armor specyfic
  Damage damageReduction() const { return _damage_red; }
  void setDamageReduction(Damage dmg_red);

//food specyfic
  int hunger() const { return _hunger; }
  void setHunger(int hunger);

private:
  Item& operator=(const Item&) = delete;
  Item(const Item&) = delete;

  Inventory _inventory;
  CreatureModificatorManager *_mods;

//item specific
  ItemType _item_type;
  std::string _name;
  std::string _descript;
  Weight _weight;
  int _value;
  ItemCondition _condition;
  int _durability;
  std::vector<BodyPartType> _bodyParts;
  bool _stackable;

//weapon specific
  WeaponSkill _wpn_skill;
  Damage _damage;
  int _defence;
  int _attack;
  int _reflex;
  int _str_req;
  int _range;

//armor specyfic
  Damage _damage_red;

//food specific
  int _hunger;

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
