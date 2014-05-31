#include "testitems.h"

using namespace std;

void TestItems::ItemCreation()
{
  //create instance item
  unique_ptr<Item> itm = Item::create(2);
  QCOMPARE(itm->ref(), static_cast<unsigned int>(2));

  //create prototype item
  bool ok = false;
  try
  {
    unique_ptr<Item> itm = Item::create(4);
  }
  catch(logic_error)
  {
    ok = true;
  }
  QVERIFY(ok);
}

void TestItems::LoadData()
{
  //create instance item
  unique_ptr<Item> itm = Item::create(2);

  //validate data
  QCOMPARE(itm->ref(), static_cast<unsigned int>(2));
  QCOMPARE(itm->value(), 32);
  QCOMPARE(itm->weight(), 1.93);
  QCOMPARE(itm->descript().c_str(), "lolo");
  QCOMPARE(itm->name().c_str(),"TestIns2");
  QVERIFY(itm->item_type() == ItemType::Armor);

}

void TestItems::PrototypeTest()
{
  unique_ptr<Item> new_item = Item::prototypes().clone(ItemPrototype::Sztylet);
  QVERIFY(new_item->ref() != 4);
  QCOMPARE(new_item->name().c_str(), "TestTpl2");
  new_item->purge();
}
