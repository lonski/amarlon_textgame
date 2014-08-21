#include "items_gateway_db.h"
#include "Equipment/item.h"
#include "Equipment/item_container.h"
#include "Include/db.h"
#include "Include/functions/db_utils.h"

using namespace std;
using namespace fun;
using namespace soci;

ItemsGatewayDB::ItemsGatewayDB()
{
}

MapRow ItemsGatewayDB::getItemDataFromDataSource(unsigned int item_id)
{
 return MapQuery( "SELECT * FROM items WHERE ref="+to_string(item_id) );
}

bool ItemsGatewayDB::itemExistsInDataSource(unsigned int item_id)
{
  short r = 0;
  soci::indicator ind;
  _Database << "select 1 from items where ref=" << item_id, into(r, ind);

  return r;
}

unsigned int ItemsGatewayDB::writeItemDataToDataSource(Item *item)
{
  try
  {
    if (itemExistsInDataSource(item->ref()))
      updateItemInDataSource(item);
    else
      insertItemIntoDataSource(item);
  }catch(soci::soci_error &e)
  {
    qDebug() << e.what();
    qDebug() << _Database.get_last_query().c_str();
  }

  return item->ref();
}

void ItemsGatewayDB::updateItemInDataSource(Item *item)
{
  stringstream save_query;

  save_query <<
    " UPDATE items SET "
     << "  ITEM_TYPE = " << static_cast<int>(item->type())
     << ", NAME = '" << item->name() << "'"
     << ", DESCRIPTION = '" << item->descript() << "'"
     << ", WEIGHT = " << item->weight()
     << ", SHOP_VALUE = " << item->value()
     << ", CONDITION = " << static_cast<int>(item->condition())
     << ", DURABILITY = " << item->durability()
     << ", BODY_PARTS = '" << item->getBodyPartsString() << "'"
     << ", STACKABLE = " << static_cast<int>(item->isStackable())
     << " ,WPN_SKILL=" << static_cast<int>(item->weaponSkill())
     << " ,WPN_D_PIERCING=" << item->damage().piercing
     << " ,WPN_D_SLASHING=" << item->damage().slashing
     << " ,WPN_D_BASHING=" << item->damage().bashing
     << " ,WPN_DEFENCE=" << item->defence()
     << " ,WPN_ATTACK=" << item->attack()
     << " ,WPN_REFLEX=" << item->reflex()
     << " ,WPN_STR_REQ=" << item->str_req()
     << " ,WPN_RANGE=" << item->range()
     << " ,ARM_DR_PIERCING=" << item->damageReduction().piercing
     << " ,ARM_DR_SLASHING=" << item->damageReduction().slashing
     << " ,ARM_DR_BASHING=" << item->damageReduction().bashing
     << " ,FOD_HUNGER=" << item->hunger()
     <<
    " WHERE ref = " << item->ref();

  _Database << save_query.str();
  _Database.commit();

}

void ItemsGatewayDB::insertItemIntoDataSource(Item *item)
{
  item->setRef(getNewItemId());

  stringstream save_query;

  save_query <<
    " INSERT INTO items (REF, ITEM_TYPE, NAME, DESCRIPTION, WEIGHT, "
                        "SHOP_VALUE, CONDITION, DURABILITY, BODY_PARTS,"
                        "STACKABLE, WPN_SKILL,WPN_D_PIERCING,WPN_D_SLASHING"
                        ",WPN_D_BASHING,WPN_DEFENCE,WPN_ATTACK,WPN_REFLEX,"
                        "WPN_STR_REQ,WPN_RANGE,ARM_DR_PIERCING,"
                        "ARM_DR_SLASHING,ARM_DR_BASHING,FOD_HUNGER)"
     << "VALUES("
     << item->ref()
     << "," <<static_cast<int>(item->type())
     << "," << "'" << item->name() << "'"
     << "," << "'" << item->descript() << "'"
     << "," << item->weight()
     << "," << item->value()
     << "," << static_cast<int>(item->condition())
     << "," << item->durability()
     << "," << "'" << item->getBodyPartsString() << "'"
     << "," << static_cast<int>(item->isStackable())
     << "," << static_cast<int>(item->weaponSkill())
     << "," << item->damage().piercing
     << "," << item->damage().slashing
     << "," << item->damage().bashing
     << "," << item->defence()
     << "," << item->attack()
     << "," << item->reflex()
     << "," << item->str_req()
     << "," << item->range()
     << "," << item->damageReduction().piercing
     << "," << item->damageReduction().slashing
     << "," << item->damageReduction().bashing
     << "," << item->hunger()
     << ")";

  _Database << save_query.str();
  _Database.commit();
}

unsigned int ItemsGatewayDB::getNewItemId()
{
  unsigned int new_ref = 0;
  soci::indicator ind;
  _Database << "select new_ref from GENERATE_ITEM_REF", into(new_ref, ind);

  return new_ref;
}

std::vector<unsigned int> ItemsGatewayDB::getItemModificatorIdsFromDataSource(unsigned int item_id)
{
  vector<unsigned int> mod_refs(100);
  vector<indicator> inds;

  string query = "SELECT ref FROM crt_mods WHERE otable='items' and oref=" + to_string(item_id);
  _Database << query, into(mod_refs, inds);

  return mod_refs;
}
