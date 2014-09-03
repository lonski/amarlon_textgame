#include "itemsgateway.h"
#include "Equipment/item.h"
#include "Equipment/item_container.h"
#include "Include/db.h"
#include "Include/functions/db_utils.h"

using namespace std;
using namespace fun;
using namespace soci;

ItemsGateway::ItemsGateway()
{
}

DBObject *ItemsGateway::fetch(dbRef id)
{
  Item* item = nullptr;

  if (itemExistsInDataSource(id))
  {
    item = new Item(id);    
    readDataIntoItem(item);
  }

  return item;
}

void ItemsGateway::fetchInto(DBObject *obj, dbRef id)
{
  Item* item = dynamic_cast<Item*>(obj);

  if (item != nullptr && itemExistsInDataSource(id))
  {
    item->setRef(id);
    readDataIntoItem(item);
  }

}

unsigned int ItemsGateway::write(DBObject *obj)
{
  Item* item = dynamic_cast<Item*>(obj);
  unsigned int r = 0;

  if (item != nullptr)
  {
    r = writeItemDataToDataSource(item);
    writeItemInventory(item);
    writeItemCrtModificators(item);
  }

  return r;
}

void ItemsGateway::copyInventory(Item* cloned, Item* item_to_clone)
{
  vector<AmountedItem> inv_to_clone = item_to_clone->inventory()->getAll();
  for (auto ai2c = inv_to_clone.begin(); ai2c != inv_to_clone.end(); ++ai2c)
  {
    AmountedItem ai_to_clone = *ai2c;
    ItemPtr to_insert( dynamic_cast<Item*>(clone(ai_to_clone.item.get())) );
    cloned->inventory()->insert(to_insert, ai_to_clone.amount);
  }
}

DBObject *ItemsGateway::clone(DBObject *to_clone)
{
  Item* item_to_clone = dynamic_cast<Item*>(to_clone);
  Item* cloned = createNewItemInstanceAndDBRecord();

  copyItemBaseData        (cloned, item_to_clone);
  copyBodyParts           (cloned, item_to_clone);
  copyCreatureModificators(cloned, item_to_clone);
  copyInventory           (cloned, item_to_clone);

  write(cloned);
  return cloned;
}

Item * ItemsGateway::createNewItemInstanceAndDBRecord()
{
  Item* cloned = new Item(0);
  write(cloned);

  return cloned;
}

void ItemsGateway::copyItemBaseData(Item* cloned, Item* item_to_clone)
{
  cloned->setName(item_to_clone->name());
  cloned->setType(item_to_clone->type());
  cloned->setDescript(item_to_clone->descript().c_str());
  cloned->setWeight(item_to_clone->weight());
  cloned->setValue(item_to_clone->value());
  cloned->setCondition(item_to_clone->condition());
  cloned->setDurability(item_to_clone->durability());
  cloned->setStackable(item_to_clone->isStackable());
  cloned->setWeaponSkill(item_to_clone->weaponSkill());
  cloned->setDamage(item_to_clone->damage());
  cloned->setDefence(item_to_clone->defence());
  cloned->setAttack(item_to_clone->attack());
  cloned->setReflex(item_to_clone->reflex());
  cloned->setStrReq(item_to_clone->str_req());
  cloned->setRange(item_to_clone->range());
  cloned->setDamageReduction(item_to_clone->damageReduction());
  cloned->setHunger(item_to_clone->hunger());
}

void ItemsGateway::copyBodyParts(Item* cloned, Item* item_to_clone)
{
  std::vector<BodyPartType> parts = item_to_clone->bodyParts();
  for (auto p = parts.begin(); p != parts.end(); ++p)
  {
    cloned->addBodyPart(*p);
  }
}

void ItemsGateway::copyCreatureModificators(Item* cloned, Item* item_to_clone)
{
  std::vector<CreatureModificator*> source_mods = item_to_clone->mods()->getAll();
  for (auto m = source_mods.begin(); m != source_mods.end(); ++m)
  {
    CreatureModificator* new_mod = new CreatureModificator;
    new_mod->augument(*(*m));
    cloned->mods()->add(new_mod);
  }
}

void ItemsGateway::readDataIntoItem(Item *item)
{
  if ( !item->loaded() && item->ref() > 0 )
  {
    MapRow item_data = getItemDataFromDataSource(item->ref());

    setItemData(item_data, item);
    setItemModificators(item);
    setItemInventory(item);

    item->set_loaded();
    item->set_not_modified();
  }
}

void ItemsGateway::setItemData(MapRow item_data, Item *item)
{
  setOrdinaryItemData(item, item_data);
  setWeaponItemData(item, item_data);
  setArmorItemData(item, item_data);
  setFoodItemData(item_data, item);
}

void ItemsGateway::setOrdinaryItemData(Item *item, MapRow item_data)
{
  item->setType( CheckValueCast<ItemType>(item_data["ITEM_TYPE"]));
  item->setName( CheckValue<string>(item_data["NAME"]) );
  item->setDescript( CheckValue<string>(item_data["DESCRIPTION"]) );
  item->setWeight( CheckValue<double>(item_data["WEIGHT"]) );
  item->setValue( CheckValue<int>(item_data["SHOP_VALUE"]) );
  item->setCondition( CheckValueCast<ItemCondition>(item_data["CONDITION"]));
  item->setBodyParts( CheckValue<string>(item_data["BODY_PARTS"]) );
  item->setDurability( CheckValue<int>(item_data["DURABILITY"]) );
  item->setStackable( CheckValue<bool>(item_data["STACKABLE"]) );
}

void ItemsGateway::setWeaponItemData(Item *item, MapRow item_data)
{
  item->setWeaponSkill(CheckValueCast<WeaponSkill>(item_data["WPN_SKILL"]));
  item->setDefence(CheckValue<int>(item_data["WPN_DEFENCE"]));
  item->setAttack(CheckValue<int>(item_data["WPN_ATTACK"]));
  item->setReflex(CheckValue<int>(item_data["WPN_REFLEX"]));
  item->setStrReq(CheckValue<int>(item_data["WPN_STR_REQ"]));
  item->setRange(CheckValue<int>(item_data["WPN_RANGE"]));

  Damage dmg
  (
    CheckValue<int>(item_data["WPN_D_PIERCING"]),
    CheckValue<int>(item_data["WPN_D_SLASHING"]),
    CheckValue<int>(item_data["WPN_D_BASHING"])
  );

  item->setDamage(dmg);
}

void ItemsGateway::setArmorItemData(Item *item, MapRow item_data)
{
  Damage dmgred
  (
    CheckValue<int>(item_data["ARM_DR_PIERCING"]),
    CheckValue<int>(item_data["ARM_DR_SLASHING"]),
    CheckValue<int>(item_data["ARM_DR_BASHING"])
  );

  item->setDamageReduction(dmgred);
}

void ItemsGateway::setFoodItemData(MapRow item_data, Item *item)
{
  item->setHunger(CheckValue<int>(item_data["FOD_HUNGER"]));
}

void ItemsGateway::writeItemInventory(Item *item)
{
  if (item->inventory()->oref() != item->ref())
  {
    item->inventory()->setORef(item->ref());
    item->inventory()->set_modified();
  }

  item->inventory()->saveToDB();
}

void ItemsGateway::writeItemCrtModificators(Item *item)
{
  item->mods()->save();
}

void ItemsGateway::setItemModificators(Item *item)
{
  item->mods()->get_complex_mod()->setName( item->name() );

  vector<unsigned int> mod_refs = getItemModificatorIdsFromDataSource(item->ref());

  for (auto m = mod_refs.begin(); m != mod_refs.end(); ++m)
    item->mods()->add( shared_ptr<CreatureModificator>(new CreatureModificator(*m)) );
}

void ItemsGateway::setItemInventory(Item *item)
{
  dbRef inv_ref = Item::Container::byOwner(item->table(), item->ref());
  if (inv_ref != 0)
  {
    item->setInventory(Item::Container::create(inv_ref));
  }
  else
  {
    item->setInventory(nullptr);
  }
}

