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
  QVERIFY(itm->type() == ItemType::Armor);
  QVERIFY(itm->condition() == ItemCondition::Good);
  QCOMPARE(itm->durability(), 7);

  QCOMPARE(itm->body_parts().size(), static_cast<size_t>(2));
  QVERIFY(itm->check_body_part(BodyPartType::LeftLeg));
  QVERIFY(itm->check_body_part(BodyPartType::Chest));

  //test weapon
  unique_ptr<Weapon> wpn( dynamic_cast<Weapon*>(Item::create(1).release()) ) ;
  QVERIFY(wpn->skill() == WeaponSkill::Miecze);
  QCOMPARE(wpn->defence(), 6);
  QCOMPARE(wpn->attack(), 2);
  QCOMPARE(wpn->reflex(), 1);
  QCOMPARE(wpn->str_req(), 3);
  QCOMPARE(wpn->range(), 5);
  QCOMPARE(wpn->damage().piercing, 1);
  QCOMPARE(wpn->damage().slashing, 2);
  QCOMPARE(wpn->damage().bashing, 3);

  //test armor
  unique_ptr<Armor> arm( dynamic_cast<Armor*>(Item::create(2).release()) ) ;
  QCOMPARE(arm->damage_reduction().piercing, 5);
  QCOMPARE(arm->damage_reduction().slashing, 6);
  QCOMPARE(arm->damage_reduction().bashing, 7);

  //test food
  unique_ptr<Food> food( dynamic_cast<Food*>(Item::create(79).release()) ) ;
  QCOMPARE(food->hunger(), 35);

  //test shield
  unique_ptr<Shield> shd( dynamic_cast<Shield*>(Item::create(82).release()) ) ;
  QCOMPARE(shd->defence(), 9);

}

void TestItems::SaveData()
{
  //create instance item
  unique_ptr<Item> itm = Item::create(2);

  //validate data
  QCOMPARE(itm->value(), 32);

  //change data
  itm->set_value(333);

  //validate change
  QCOMPARE(itm->value(), 333);

  //save
  itm->save_to_db();

  //invoke desctuctor as save
  Item* item = itm.release();
  delete item;

  //load again
  itm = Item::create(2);

  //validate saved
  QCOMPARE(itm->value(), 333);

  //rollback change
  itm->set_value(32);

  //load again
  itm->save_to_db();
  itm->reload();

  //validate rollback
  QCOMPARE(itm->value(), 32);

  //===========================
  //#############fast check all fields
  itm->set_weight(10);
  itm->set_descript("x");
  itm->set_name("xx");
  itm->set_type(ItemType::Food);
  itm->set_condition(ItemCondition::Damaged);
  itm->add_body_part(BodyPartType::Head);
  itm->remove_body_part(BodyPartType::Chest);
  itm->set_durability(5);

  //
  delete itm.release();
  itm = Item::create(2);

  //##########validate save

  QCOMPARE(itm->weight(), 10.0);
  QCOMPARE(itm->descript().c_str(), "x");
  QCOMPARE(itm->name().c_str(),"xx");
  QVERIFY(itm->type() == ItemType::Food);
  QVERIFY(itm->condition() == ItemCondition::Damaged);
  QCOMPARE(itm->body_parts().size(), static_cast<size_t>(2));
  QVERIFY(itm->check_body_part(BodyPartType::Head));
  QVERIFY(itm->check_body_part(BodyPartType::LeftLeg));
  QCOMPARE(itm->durability(), 5);

  //rollback
  itm->set_weight(1.93);
  itm->set_descript("lolo");
  itm->set_name("TestIns2");
  itm->set_type(ItemType::Armor);
  itm->set_condition(ItemCondition::Good);
  itm->add_body_part(BodyPartType::Chest);
  itm->remove_body_part(BodyPartType::Head);
  itm->set_durability(7);

  delete itm.release();

  //=============test weapon specific save
  unique_ptr<Weapon> wpn(dynamic_cast<Weapon*>(Item::create(1).release()));

  //change data
  wpn->set_skill(WeaponSkill::Kije);
  wpn->set_defence(7);
  wpn->set_attack(3);
  wpn->set_reflex(2);
  wpn->set_str_req(4);
  wpn->set_range(6);
  wpn->set_damage(Damage(2,3,4));

  //re-create
  delete wpn.release();
  wpn = unique_ptr<Weapon>(dynamic_cast<Weapon*>(Item::create(1).release()));

  //validate
  QVERIFY(wpn->skill() == WeaponSkill::Kije);
  QCOMPARE(wpn->defence(), 7);
  QCOMPARE(wpn->attack(), 3);
  QCOMPARE(wpn->reflex(), 2);
  QCOMPARE(wpn->str_req(), 4);
  QCOMPARE(wpn->range(), 6);
  QCOMPARE(wpn->damage().piercing, 2);
  QCOMPARE(wpn->damage().slashing, 3);
  QCOMPARE(wpn->damage().bashing, 4);

  //rollback
  wpn->set_skill(WeaponSkill::Miecze);
  wpn->set_defence(6);
  wpn->set_attack(2);
  wpn->set_reflex(1);
  wpn->set_str_req(3);
  wpn->set_range(5);
  wpn->set_damage(Damage(1,2,3));

  delete wpn.release();

  //test armor specific ========================
  unique_ptr<Armor> arm(dynamic_cast<Armor*>(Item::create(2).release()));

  //change data
  arm->set_damage_reduction(Damage(2,3,4));

  //re-create
  delete arm.release();
  arm = unique_ptr<Armor>(dynamic_cast<Armor*>(Item::create(2).release()));

  //validate
  QCOMPARE(arm->damage_reduction().piercing, 2);
  QCOMPARE(arm->damage_reduction().slashing, 3);
  QCOMPARE(arm->damage_reduction().bashing, 4);

  //rollback
  arm->set_damage_reduction(Damage(5,6,7));

  delete arm.release();

  //test food specyfic===============================
  unique_ptr<Food> food(dynamic_cast<Food*>(Item::create(79).release()));

  //change data
  food->set_hunger(20);

  //re-create
  delete food.release();
  food = unique_ptr<Food>(dynamic_cast<Food*>(Item::create(79).release()));

  //validate
  QCOMPARE(food->hunger(), 20);

  //rollback
  food->set_hunger(35);

  delete food.release();

  //test shield specyfic===============================
  unique_ptr<Shield> shd(dynamic_cast<Shield*>(Item::create(82).release()));

  //change data
  shd->set_defence(11);

  //re-create
  delete shd.release();
  shd = unique_ptr<Shield>(dynamic_cast<Shield*>(Item::create(82).release()));

  //validate
  QCOMPARE(shd->defence(), 11);

  //rollback
  shd->set_defence(9);

  delete shd.release();

}

void TestItems::TemporaryItem()
{
  //create instance item
  unique_ptr<Item> itm = Item::create(2, false, true);

  //validate data
  QVERIFY(itm->isTemporary());
  QCOMPARE(itm->ref(), static_cast<unsigned int>(2));
  QCOMPARE(itm->value(), 32);
  QCOMPARE(itm->weight(), 1.93);
  QCOMPARE(itm->descript().c_str(), "lolo");
  QCOMPARE(itm->name().c_str(),"TestIns2");
  QVERIFY(itm->type() == ItemType::Armor);

  //change some data
  itm->set_weight(6.66);

  //save
  itm->save_to_db();

  //load again
  itm->reload();

  //validate if not changed
  QCOMPARE(itm->weight(), 1.93);

}

void TestItems::PrototypeTest()
{
  unique_ptr<Item> new_item = Item::prototypes().clone(ItemPrototype::Sztylet);
  QVERIFY(new_item->ref() != 4);
  QCOMPARE(new_item->name().c_str(), "TestTpl2");
  new_item->purge();
}
