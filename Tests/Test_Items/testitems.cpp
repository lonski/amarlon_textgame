#include "testitems.h"
#include "Equipment/item_container.h"
#include "Include/enums/e_refdict.h"

using namespace std;

void TestItems::ItemCreation()
{
  //create instance item
  unique_ptr<Item> itm( Item::create(2));
  QCOMPARE(itm->ref(), static_cast<unsigned int>(2));
}

void TestItems::LoadData()
{
  //create instance item
  unique_ptr<Item> itm (Item::create(2));

  //validate data
  QCOMPARE(itm->ref(), static_cast<unsigned int>(2));
  QCOMPARE(itm->value(), 32);
  QCOMPARE(itm->weight(), 1.93);
  QCOMPARE(itm->descript().c_str(), "lolo");
  QCOMPARE(itm->name().c_str(),"TestIns2");
  QVERIFY(itm->type() == ItemType::Armor);
  QVERIFY(itm->condition() == ItemCondition::Good);
  QCOMPARE(itm->durability(), 7);

  QCOMPARE(itm->bodyParts().size(), static_cast<size_t>(2));
  QVERIFY(itm->checkBodyPart(BodyPartType::Glowa));
  QVERIFY(itm->checkBodyPart(BodyPartType::Tors));

  //test weapon
  unique_ptr<Item> wpn( Item::create(1) ) ;
  QVERIFY(wpn->weaponSkill() == WeaponSkill::Miecze);
  QCOMPARE(wpn->defence(), 6);
  QCOMPARE(wpn->attack(), 2);
  QCOMPARE(wpn->reflex(), 1);
  QCOMPARE(wpn->str_req(), 3);
  QCOMPARE(wpn->range(), 5);
  QCOMPARE(wpn->damage().piercing, 1);
  QCOMPARE(wpn->damage().slashing, 2);
  QCOMPARE(wpn->damage().bashing, 3);

  //test armor
  unique_ptr<Item> arm(Item::create(2)) ;
  QCOMPARE(arm->damageReduction().piercing, 5);
  QCOMPARE(arm->damageReduction().slashing, 6);
  QCOMPARE(arm->damageReduction().bashing, 7);

  //test food
  unique_ptr<Item> food( (Item::create(79)) ) ;
  QCOMPARE(food->hunger(), 35);

}

void TestItems::SaveData()
{
  //create instance item
  unique_ptr<Item> itm (Item::create(2));

  //validate data
  QCOMPARE(itm->value(), 32);

  //change data
  itm->setValue(333);

  //validate change
  QCOMPARE(itm->value(), 333);

  //save
  itm->saveToDB();

  //invoke desctuctor as save
  Item* item = itm.release();
  delete item;

  //load again
  itm.reset( Item::create(2) );

  //validate saved
  QCOMPARE(itm->value(), 333);

  //rollback change
  itm->setValue(32);

  //load again
  itm->saveToDB();
  itm->reload();

  //validate rollback
  QCOMPARE(itm->value(), 32);

  //===========================
  //#############fast check all fields
  itm->setWeight(10);
  itm->setDescript("x");
  itm->setName("xx");
  itm->setType(ItemType::Food);
  itm->setCondition(ItemCondition::Damaged);
  itm->addBodyPart(BodyPartType::Reka);
  itm->removeBodyPart(BodyPartType::Tors);
  itm->setDurability(5);

  //
  delete itm.release();
  itm.reset( Item::create(2) );

  //##########validate save

  QCOMPARE(itm->weight(), 10.0);
  QCOMPARE(itm->descript().c_str(), "x");
  QCOMPARE(itm->name().c_str(),"xx");
  QVERIFY(itm->type() == ItemType::Food);
  QVERIFY(itm->condition() == ItemCondition::Damaged);
  QCOMPARE(itm->bodyParts().size(), static_cast<size_t>(2));
  QVERIFY(itm->checkBodyPart(BodyPartType::Glowa));
  QVERIFY(itm->checkBodyPart(BodyPartType::Reka));
  QCOMPARE(itm->durability(), 5);

  //rollback
  itm->setWeight(1.93);
  itm->setDescript("lolo");
  itm->setName("TestIns2");
  itm->setType(ItemType::Armor);
  itm->setCondition(ItemCondition::Good);
  itm->addBodyPart(BodyPartType::Tors);
  itm->removeBodyPart(BodyPartType::Reka);
  itm->setDurability(7);

  delete itm.release();

  //=============test weapon specific save
  unique_ptr<Item> wpn((Item::create(1)));

  //change data
  wpn->setWeaponSkill(WeaponSkill::Kije);
  wpn->setDefence(7);
  wpn->setAttack(3);
  wpn->setReflex(2);
  wpn->setStrReq(4);
  wpn->setRange(6);
  wpn->setDamage(Damage(2,3,4));

  //re-create
  delete wpn.release();
  wpn = unique_ptr<Item>((Item::create(1)));

  //validate
  QVERIFY(wpn->weaponSkill() == WeaponSkill::Kije);
  QCOMPARE(wpn->defence(), 7);
  QCOMPARE(wpn->attack(), 3);
  QCOMPARE(wpn->reflex(), 2);
  QCOMPARE(wpn->str_req(), 4);
  QCOMPARE(wpn->range(), 6);
  QCOMPARE(wpn->damage().piercing, 2);
  QCOMPARE(wpn->damage().slashing, 3);
  QCOMPARE(wpn->damage().bashing, 4);

  //rollback
  wpn->setWeaponSkill(WeaponSkill::Miecze);
  wpn->setDefence(6);
  wpn->setAttack(2);
  wpn->setReflex(1);
  wpn->setStrReq(3);
  wpn->setRange(5);
  wpn->setDamage(Damage(1,2,3));

  delete wpn.release();

  //test armor specific ========================
  unique_ptr<Item> arm((Item::create(2)));

  //change data
  arm->setDamageReduction(Damage(2,3,4));

  //re-create
  delete arm.release();
  arm = unique_ptr<Item>((Item::create(2)));

  //validate
  QCOMPARE(arm->damageReduction().piercing, 2);
  QCOMPARE(arm->damageReduction().slashing, 3);
  QCOMPARE(arm->damageReduction().bashing, 4);

  //rollback
  arm->setDamageReduction(Damage(5,6,7));

  delete arm.release();

  //test food specyfic===============================
  unique_ptr<Item> food((Item::create(79)));

  //change data
  food->setHunger(20);

  //re-create
  delete food.release();
  food = unique_ptr<Item>((Item::create(79)));

  //validate
  QCOMPARE(food->hunger(), 20);

  //rollback
  food->setHunger(35);

  delete food.release();

  //test shield specyfic===============================
  unique_ptr<Item> shd((Item::create(82)));

  //change data
  shd->setDefence(11);

  //re-create
  delete shd.release();
  shd = unique_ptr<Item>((Item::create(82)));

  //validate
  QCOMPARE(shd->defence(), 11);

  //rollback
  shd->setDefence(9);

  delete shd.release();

}


void TestItems::PrototypeTest()
{
  unique_ptr<Item> new_item (Item::prototypes().clone(ItemPrototype::Nozyk));
  QVERIFY(new_item->ref() != 4);
  QCOMPARE(new_item->name().c_str(), "TestTpl2");
  new_item->purge();
}

void TestItems::ContainerCreation()
{
  Item::Container* cont = Item::Container::create(1);
  QCOMPARE(cont->name().c_str(), "Ekwipunek");
  delete cont;
}

void TestItems::ContainerInsertionEraseNonStackable()
{
  //===================INSERT===================
  //create some new container
  Item::Container* cont = Item::Container::create();
  cont->setName("TestCont");
  QCOMPARE(cont->name().c_str(), "TestCont");

  //check content
  QCOMPARE(cont->getAll().size(), static_cast<size_t>(0));

  //create some new items
  ItemPtr item1( Item::prototypes().clone(ItemPrototype::Nozyk) );
  ItemPtr item2( Item::prototypes().clone(ItemPrototype::Sztylet_typowy) );

  //insert them
  cont->insert(item1);
  cont->insert(item2);

  //check content
  QCOMPARE(cont->getAll().size(), static_cast<size_t>(2));

  //recreate and check content
  dbRef cont_ref = cont->ref();

  delete cont;
  cont = Item::Container::create(cont_ref);
  QCOMPARE(cont->getAll().size(), static_cast<size_t>(2));

  //====================ERASE======================
  cont->erase(item1->ref());
  QCOMPARE(cont->getAll().size(), static_cast<size_t>(1));
  cont->erase(item2->ref());
  QCOMPARE(cont->getAll().size(), static_cast<size_t>(0));

  //recreate and check content
  delete cont;
  cont = Item::Container::create(cont_ref);
  QCOMPARE(cont->getAll().size(), static_cast<size_t>(0));

  //clean up
  cont->purge();
  item1->purge();
  item2->purge();
  delete cont;

}

void TestItems::ContainerInsertionEraseStackable()
{
  //create some new container
  Item::Container* cont = Item::Container::create();
  cont->setName("TestCont");
  QCOMPARE(cont->name().c_str(), "TestCont");

  //check content
  QCOMPARE(cont->getAll().size(), static_cast<size_t>(0));

  //create some new items
  ItemPtr miedziak( Item::create( (int)refDict::Item::Miedziak ) );
  QVERIFY(miedziak->isStackable());

  //insert
  cont->insert(miedziak, 10);

  //validate
  QCOMPARE(cont->getAll().size(), static_cast<size_t>(1));
  QVERIFY(cont->find(miedziak->ref()).item.get() != nullptr);
  QCOMPARE(cont->find(miedziak->ref()).amount, 10);
  cont->reload();
  QCOMPARE(cont->getAll().size(), static_cast<size_t>(1));
  QVERIFY(cont->find(miedziak->ref()).item.get() != nullptr);
  QCOMPARE(cont->find(miedziak->ref()).amount, 10);

  //insert more
  cont->insert(miedziak, 4);

  //validate
  cont->reload();
  QCOMPARE(cont->getAll().size(), static_cast<size_t>(1));
  QVERIFY(cont->find(miedziak->ref()).item.get() != nullptr);
  QCOMPARE(cont->find(miedziak->ref()).amount, 14);

  //insert even more and some other item
  ItemPtr item1( Item::prototypes().clone(ItemPrototype::Nozyk) );
  cont->insert(item1);
  cont->insert(miedziak, 2);

  //validate
  cont->reload();
  QCOMPARE(cont->getAll().size(), static_cast<size_t>(2));
  QVERIFY(cont->find(miedziak->ref()).item.get() != nullptr);
  QCOMPARE(cont->find(miedziak->ref()).amount, 16);

  //erase some
  cont->erase(miedziak->ref(), 12);

  //validate
  cont->reload();
  QCOMPARE(cont->getAll().size(), static_cast<size_t>(2));
  QVERIFY(cont->find(miedziak->ref()).item.get() != nullptr);
  QCOMPARE(cont->find(miedziak->ref()).amount, 4);

  //erase all miedziaks
  cont->erase(miedziak->ref(), 4);

  //validate
  cont->reload();
  QCOMPARE(cont->getAll().size(), static_cast<size_t>(1));
  QVERIFY(cont->find(miedziak->ref()).item.get() == nullptr);

  //clean up
  cont->purge();
  item1->purge();
  delete cont;
}

void TestItems::ItemAsAContainer()
{
  //create some item
  Item* szkatulka = Item::prototypes().clone(ItemPrototype::Null);
  szkatulka->setName("Szkatułka");

  //create a container for that item
  Item::Container* cont = Item::Container::create();
  cont->set_max_weight(5);
  cont->setOTable(szkatulka->table());
  cont->setORef(szkatulka->ref());
  cont->saveToDB();
  delete cont; //is not needed anymore -> will be loaded on item reload

  //it should apply new container to item
  szkatulka->reload();

  QVERIFY(szkatulka->inventory().get() != nullptr);

  //create some item to insert to szkatułka
  ItemPtr i1( Item::create( (int)refDict::Item::Miedziak) );
  ItemPtr i2( Item::prototypes().clone(ItemPrototype::Sztylet_typowy) );

  //insert to szkatułka
  szkatulka->inventory()->insert(i1, 5);
  szkatulka->inventory()->insert(i2);

  QCOMPARE(szkatulka->inventory()->getAll().size(), (size_t)2 );

  //clean up!
  szkatulka->inventory()->purge();
  i2->purge();
  szkatulka->purge();
  delete szkatulka;

}

void TestItems::ItemModManager()
{
  //create some item
  unique_ptr<Item> item (Item::prototypes().clone(ItemPrototype::Nozyk));
  dbRef iref = item->ref();

  //=================
  //create some mods
  shared_ptr<CreatureModificator> mod1(new CreatureModificator(item->table(), item->ref()))
                                , mod2(new CreatureModificator(item->table(), item->ref()))
                                , mod3(new CreatureModificator(item->table(), item->ref()));

  //set the mods and save to DB
  mod1->creature_stats().setAttribute(Attribute::CHR, 1);
  mod1->creature_stats().setAttribute(Attribute::STR, 1);
  mod1->saveToDB();

  mod2->creature_stats().setAttribute(Attribute::CHR, 2);
  mod2->creature_stats().setAttribute(Attribute::DEX, 1);
  mod2->saveToDB();

  mod3->creature_stats().setAttribute(Attribute::CHR, 3);
  mod3->creature_stats().setAttribute(Attribute::IMP, 1);
  mod3->saveToDB();

  //reset item
  delete item.release();
  item.reset(Item::create(iref));
  Item *itm = item.get(); //bo nie podpowiada jak wyłuskuję unika. denerwujące.

  //VALIDATE MOD MANAGER

  //validate size
  QCOMPARE(itm->mods()->getAll().size(), (size_t)3);
  //validate complex mod
  QCOMPARE(itm->mods()->get_complex_mod()->creature_stats().attribute(Attribute::CHR), 6);
  QCOMPARE(itm->mods()->get_complex_mod()->creature_stats().attribute(Attribute::STR), 1);
  QCOMPARE(itm->mods()->get_complex_mod()->creature_stats().attribute(Attribute::DEX), 1);
  QCOMPARE(itm->mods()->get_complex_mod()->creature_stats().attribute(Attribute::IMP), 1);

  //REMOVE mod2
  itm->mods()->remove(mod2->ref());

  //validate size
  QCOMPARE(itm->mods()->getAll().size(), (size_t)2);
  //validate complex mod
  QCOMPARE(itm->mods()->get_complex_mod()->creature_stats().attribute(Attribute::CHR), 4);
  QCOMPARE(itm->mods()->get_complex_mod()->creature_stats().attribute(Attribute::STR), 1);
  QCOMPARE(itm->mods()->get_complex_mod()->creature_stats().attribute(Attribute::DEX), 0);
  QCOMPARE(itm->mods()->get_complex_mod()->creature_stats().attribute(Attribute::IMP), 1);

  itm->purge();
}
