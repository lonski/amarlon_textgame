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
  QVERIFY(itm->check_body_part(BodyPartType::Glowa));
  QVERIFY(itm->check_body_part(BodyPartType::Tors));

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
  itm->add_body_part(BodyPartType::Reka);
  itm->remove_body_part(BodyPartType::Tors);
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
  QVERIFY(itm->check_body_part(BodyPartType::Glowa));
  QVERIFY(itm->check_body_part(BodyPartType::Reka));
  QCOMPARE(itm->durability(), 5);

  //rollback
  itm->set_weight(1.93);
  itm->set_descript("lolo");
  itm->set_name("TestIns2");
  itm->set_type(ItemType::Armor);
  itm->set_condition(ItemCondition::Good);
  itm->add_body_part(BodyPartType::Tors);
  itm->remove_body_part(BodyPartType::Reka);
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
  //rollback error in save item
  unique_ptr<Item> itmx = Item::create(2);

  itmx->set_weight(1.93);
  itmx->set_descript("lolo");
  itmx->set_name("TestIns2");
  itmx->set_type(ItemType::Armor);
  itmx->set_condition(ItemCondition::Good);

  vector<BodyPartType> bps = itmx->body_parts();
  for_each(bps.begin(), bps.end(), [&](BodyPartType b)
  {
    itmx->remove_body_part(b);
  });

  itmx->add_body_part(BodyPartType::Tors);
  itmx->add_body_part(BodyPartType::Glowa);
  itmx->set_durability(7);
  delete itmx.release();
  //~~~

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
  unique_ptr<Item> new_item = Item::prototypes().clone(ItemPrototype::Nozyk);
  QVERIFY(new_item->ref() != 4);
  QCOMPARE(new_item->name().c_str(), "TestTpl2");
  new_item->purge();
}

void TestItems::ContainerCreation()
{
  unique_ptr<Item::Container<> > cont = Item::Container<>::create(1,true);
  QCOMPARE(cont->name().c_str(), "Ekwipunek");
}

void TestItems::ContainerCreationFromPrototype()
{
  unique_ptr<Item::Container<> > cont = Item::Container<>::prototypes().clone(ItemContainerPrototype::Inventory);
  QVERIFY(cont->ref() != 1);
  QCOMPARE(cont->name().c_str(), "Ekwipunek");
  cont->purge();
}

void TestItems::ContainerInsertionEraseNonStackable()
{
  //===================INSERT===================
  //create some new container
  unique_ptr<Item::Container<> > cont = Item::Container<>::prototypes().clone(ItemContainerPrototype::Inventory);
  cont->set_name("TestCont");
  QCOMPARE(cont->name().c_str(), "TestCont");

  //check content
  QCOMPARE(cont->get_all().size(), static_cast<size_t>(0));

  //create some new items
  shared_ptr<Item> item1( Item::prototypes().clone(ItemPrototype::Nozyk).release() );
  shared_ptr<Item> item2( Item::prototypes().clone(ItemPrototype::Sztylet_typowy).release() );

  //insert them
  cont->insert(item1);
  cont->insert(item2);

  //check content
  QCOMPARE(cont->get_all().size(), static_cast<size_t>(2));

  //recreate and check content
  dbRef cont_ref = cont->ref();
  delete cont.release();
  cont = Item::Container<>::create(cont_ref);
  QCOMPARE(cont->get_all().size(), static_cast<size_t>(2));

  //====================ERASE======================
  cont->erase(item1->ref());
  QCOMPARE(cont->get_all().size(), static_cast<size_t>(1));
  cont->erase(item2->ref());
  QCOMPARE(cont->get_all().size(), static_cast<size_t>(0));

  //recreate and check content
  delete cont.release();
  cont = Item::Container<>::create(cont_ref);
  QCOMPARE(cont->get_all().size(), static_cast<size_t>(0));

  //clean up
  cont->purge();
  item1->purge();
  item2->purge();

}

void TestItems::ContainerInsertionEraseStackable()
{
  //create some new container
  unique_ptr<Item::Container<> > cont = Item::Container<>::prototypes().clone(ItemContainerPrototype::Inventory);
  cont->set_name("TestCont");
  QCOMPARE(cont->name().c_str(), "TestCont");

  //check content
  QCOMPARE(cont->get_all().size(), static_cast<size_t>(0));

  //create some new items
  shared_ptr<Item> miedziak( Item::create( (int)refDict::Item::Miedziak ) );
  QVERIFY(miedziak->isStackable());

  //insert
  cont->insert(miedziak, 10);

  //validate
  cont->reload();
  QCOMPARE(cont->get_all().size(), static_cast<size_t>(1));
  QVERIFY(cont->find(miedziak->ref()).item.get() != nullptr);
  QCOMPARE(cont->find(miedziak->ref()).amount, 10);

  //insert more
  cont->insert(miedziak, 4);

  //validate
  cont->reload();
  QCOMPARE(cont->get_all().size(), static_cast<size_t>(1));
  QVERIFY(cont->find(miedziak->ref()).item.get() != nullptr);
  QCOMPARE(cont->find(miedziak->ref()).amount, 14);

  //insert even more and some other item
  shared_ptr<Item> item1( Item::prototypes().clone(ItemPrototype::Nozyk).release() );
  cont->insert(item1);
  cont->insert(miedziak, 2);

  //validate
  cont->reload();
  QCOMPARE(cont->get_all().size(), static_cast<size_t>(2));
  QVERIFY(cont->find(miedziak->ref()).item.get() != nullptr);
  QCOMPARE(cont->find(miedziak->ref()).amount, 16);

  //erase some
  cont->erase(miedziak->ref(), 12);

  //validate
  cont->reload();
  QCOMPARE(cont->get_all().size(), static_cast<size_t>(2));
  QVERIFY(cont->find(miedziak->ref()).item.get() != nullptr);
  QCOMPARE(cont->find(miedziak->ref()).amount, 4);

  //erase all miedziaks
  cont->erase(miedziak->ref(), 4);

  //validate
  cont->reload();
  QCOMPARE(cont->get_all().size(), static_cast<size_t>(1));
  QVERIFY(cont->find(miedziak->ref()).item.get() == nullptr);

  //clean up
  cont->purge();
  item1->purge();
}

void TestItems::ItemAsAContainer()
{
  //create some item
  Item* szkatulka = Item::prototypes().clone(ItemPrototype::Null).release();
  szkatulka->set_name("Szkatułka");

  //create a container for that item
  Item::Container<>* cont = Item::Container<>::prototypes().clone(ItemContainerPrototype::Null).release();
  cont->set_max_weight(5);
  cont->set_otable(szkatulka->table());
  cont->set_oref(szkatulka->ref());
  cont->save_to_db();
  delete cont; //is not needed anymore -> will be loaded on item reload

  //it should apply new container to item
  szkatulka->reload();

  QVERIFY(szkatulka->inventory().get() != nullptr);

  //create some item to insert to szkatułka
  shared_ptr<Item> i1( Item::create( (int)refDict::Item::Miedziak).release() );
  shared_ptr<Item> i2( Item::prototypes().clone(ItemPrototype::Sztylet_typowy) );

  //insert to szkatułka
  szkatulka->inventory()->insert(i1, 5);
  szkatulka->inventory()->insert(i2);

  QCOMPARE(szkatulka->inventory()->get_all().size(), (size_t)2 );

  //clean up!
  szkatulka->inventory()->purge();
  i2->purge();
  szkatulka->purge();
  delete szkatulka;

}

void TestItems::ItemModManager()
{
  //create some item
  unique_ptr<Item> item = Item::prototypes().clone(ItemPrototype::Nozyk);
  dbRef iref = item->ref();

  //=================
  //create some mods
  shared_ptr<CreatureModificator> mod1(new CreatureModificator(item->table(), item->ref()))
                                , mod2(new CreatureModificator(item->table(), item->ref()))
                                , mod3(new CreatureModificator(item->table(), item->ref()));

  //set the mods and save to DB
  mod1->creature_stats().set_attribute(Attribute::CHR, 1);
  mod1->creature_stats().set_attribute(Attribute::STR, 1);
  mod1->save_to_db();

  mod2->creature_stats().set_attribute(Attribute::CHR, 2);
  mod2->creature_stats().set_attribute(Attribute::DEX, 1);
  mod2->save_to_db();

  mod3->creature_stats().set_attribute(Attribute::CHR, 3);
  mod3->creature_stats().set_attribute(Attribute::IMP, 1);
  mod3->save_to_db();

  //reset item
  delete item.release();
  item = Item::create(iref);
  Item *itm = item.get(); //bo nie podpowiada jak wyłuskuję unika. denerwujące.

  //VALIDATE MOD MANAGER

  //validate size
  QCOMPARE(itm->mods().get_all().size(), (size_t)3);
  //validate complex mod
  QCOMPARE(itm->mods().get_complex_mod()->creature_stats().attribute(Attribute::CHR), 6);
  QCOMPARE(itm->mods().get_complex_mod()->creature_stats().attribute(Attribute::STR), 1);
  QCOMPARE(itm->mods().get_complex_mod()->creature_stats().attribute(Attribute::DEX), 1);
  QCOMPARE(itm->mods().get_complex_mod()->creature_stats().attribute(Attribute::IMP), 1);

  //REMOVE mod2
  itm->mods().remove(mod2->ref());

  //validate size
  QCOMPARE(itm->mods().get_all().size(), (size_t)2);
  //validate complex mod
  QCOMPARE(itm->mods().get_complex_mod()->creature_stats().attribute(Attribute::CHR), 4);
  QCOMPARE(itm->mods().get_complex_mod()->creature_stats().attribute(Attribute::STR), 1);
  QCOMPARE(itm->mods().get_complex_mod()->creature_stats().attribute(Attribute::DEX), 0);
  QCOMPARE(itm->mods().get_complex_mod()->creature_stats().attribute(Attribute::IMP), 1);

  itm->purge();
}
