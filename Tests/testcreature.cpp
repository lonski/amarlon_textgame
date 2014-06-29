#include "testcreature.h"

using namespace std;

void TestCreature::stats_atr2str()
{
  CreatureStats stats;

  stats.set_attribute(Attribute::CHR, 2);
  stats.set_attribute(Attribute::IMP, 5);
  stats.set_attribute(Attribute::STR, 7);
  stats.set_attribute(Attribute::DEX, 3);

  QCOMPARE(stats.Attributes2Str().c_str(), "0;7;3;0;0;0;2;5;0;");
}

void TestCreature::stats_str2atrs()
{
  CreatureStats stats;

  stats.Str2Attributes("0;7;3;9;0;0;2;5;0;");

  QCOMPARE(stats.get_attribute(Attribute::CHR), 2);
  QCOMPARE(stats.get_attribute(Attribute::IMP), 5);
  QCOMPARE(stats.get_attribute(Attribute::STR), 7);
  QCOMPARE(stats.get_attribute(Attribute::DEX), 3);
  QCOMPARE(stats.get_attribute(Attribute::END), 9);
  QCOMPARE(stats.get_attribute(Attribute::REF), 0);
  QCOMPARE(stats.get_attribute(Attribute::INT), 0);
  QCOMPARE(stats.get_attribute(Attribute::WLL), 0);
}

void TestCreature::stats_skills2str()
{
  CreatureStats stats;

  stats.set_skill(Skill::Aktorstwo, 43);
  stats.set_skill(Skill::Empatia, 12);
  stats.set_skill(Skill::Mocna_glowa, 67);

  QCOMPARE( stats.Skills2Str().c_str(), "0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;12;0;0;0;0;0;0;0;0;43;0;0;0;0;0;0;0;0;0;0;0;67;0;0;0;0;0;0;");
}

void TestCreature::stats_str2skills()
{
  CreatureStats stats;

  stats.Str2Skills("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;12;0;0;0;0;0;0;0;0;43;0;0;0;0;0;0;0;0;0;0;0;67;0;0;0;0;0;1;");

  QCOMPARE(stats.get_skill(Skill::Aktorstwo), 43);
  QCOMPARE(stats.get_skill(Skill::Empatia), 12);
  QCOMPARE(stats.get_skill(Skill::Mocna_glowa), 67);
  QCOMPARE(stats.get_skill(Skill::Taniec), 1);
  QCOMPARE(stats.get_skill(Skill::Taktyka), 0);
}

void TestCreature::bodypart_tostr()
{
  BodyPart bp;
  bp.set_region(BodyRegion::Dol);
  bp.set_side(BodySide::Left);
  bp.set_type(BodyPartType::Noga);
  bp.set_damage(DamageLevel::Brak);

  QCOMPARE( bp.toStr().c_str(), "2,1,4,1,0");
}

void TestCreature::bodypart_fromstr()
{
  BodyPart bp("2,1,4,1,0");

  QVERIFY(bp.region() == BodyRegion::Dol);
  QVERIFY(bp.side() == BodySide::Left);
  QVERIFY(bp.type() == BodyPartType::Noga);
  QVERIFY(bp.damage() == DamageLevel::Brak);
  QVERIFY(bp.equipped().lock() == nullptr);
}

void TestCreature::bodypart_equip()
{
  BodyPart bp("2,1,4,1,0");
  shared_ptr<Item> item( Item::prototypes().clone(ItemPrototype::Noz).release() );

  bp.equip(item);
  QVERIFY(bp.equipped().lock() != nullptr);

  string str = "2,1,4,1,"+fun::toStr(item->ref());
  QCOMPARE(bp.toStr().c_str(), str.c_str());

  item->purge();
}

void TestCreature::bodypart_unequip()
{
  //equip
  BodyPart bp("2,1,4,1,0");
  shared_ptr<Item> item( Item::prototypes().clone(ItemPrototype::Noz).release() );

  bp.equip(item);
  QVERIFY(bp.equipped().lock() != nullptr);

  string str = "2,1,4,1,"+fun::toStr(item->ref());
  QCOMPARE(bp.toStr().c_str(), str.c_str());
  //~~~

  shared_ptr<Item> i2 = bp.unequip();
  QVERIFY(bp.equipped().lock() == nullptr);
  QCOMPARE(i2.get(), item.get());

  item->purge();
}

void TestCreature::bodypart_creation_fromstr()
{
  BodyPart bp("2,1,4,1,121");

  QVERIFY(bp.region() == BodyRegion::Dol);
  QVERIFY(bp.side() == BodySide::Left);
  QVERIFY(bp.type() == BodyPartType::Noga);
  QVERIFY(bp.damage() == DamageLevel::Brak);
  QVERIFY(bp.equipped().lock() != nullptr);
  QCOMPARE(bp.equipped().lock()->ref(), (dbRef)121);
  QCOMPARE(bp.toStr().c_str(), "2,1,4,1,121");
}

void TestCreature::modificator_creation()
{
  CreatureModificator mod;
  mod.save_to_db();
  QVERIFY(mod.ref() != 0);
  mod.purge();
}

void TestCreature::modificator_creation_existing()
{
  CreatureModificator mod(13);
  QCOMPARE(mod.ref(), (dbRef)13);
}

void TestCreature::modificator_load()
{
  CreatureModificator mod(13);

  QCOMPARE(mod.name().c_str(), "testo");
  QCOMPARE(mod.global_test_level_mod(), 1);
  QCOMPARE(mod.effect_time(), 9);
  QCOMPARE(mod.creature_stats().get_attribute(Attribute::CHR), 3);
  QCOMPARE(mod.creature_stats().get_attribute(Attribute::STR), 0);
  QCOMPARE(mod.creature_stats().get_skill(Skill::Czujnosc), 34);
  QCOMPARE(mod.creature_stats().get_skill(Skill::Akrobatyka), 0);
}

void TestCreature::modificator_save()
{
  //create blank one
  CreatureModificator* mod = new CreatureModificator;

  //create record in db
  mod->save_to_db();

  //set some data
  mod->creature_stats().set_attribute(Attribute::DEX, 3);
  mod->set_name("lol");

  //store ref
  dbRef ref = mod->ref();

  delete mod;

  //recreate
  mod = new CreatureModificator(ref);

  //validate
  QCOMPARE(mod->name().c_str(), "lol");
  QVERIFY(mod->creature_stats().get_attribute(Attribute::DEX) == 3);

  //clean up!
  mod->purge();
}

void TestCreature::modificator_augument()
{
  CreatureModificator m1, m2;

  //set some stats
  m1.creature_stats().set_attribute(Attribute::IMP, 3);
  m1.creature_stats().set_skill(Skill::Gornictwo, 34);
  m1.set_global_test_level_mod(1);

  QCOMPARE(m1.creature_stats().get_attribute(Attribute::IMP), 3);
  QCOMPARE(m1.creature_stats().get_skill(Skill::Gornictwo), 34);
  QCOMPARE(m1.global_test_level_mod(), 1);
  QCOMPARE(m1.effect_time(), -1);

  //set augument stats
  m2.creature_stats().set_attribute(Attribute::IMP, 1);
  m2.creature_stats().set_attribute(Attribute::STR, 3);
  m2.creature_stats().set_skill(Skill::Odpornosc, 4);
  m2.set_global_test_level_mod(2);
  m2.set_effect_time(50);

  QCOMPARE(m2.creature_stats().get_attribute(Attribute::IMP), 1);
  QCOMPARE(m2.creature_stats().get_attribute(Attribute::STR), 3);
  QCOMPARE(m2.creature_stats().get_skill(Skill::Odpornosc), 4);
  QCOMPARE(m2.global_test_level_mod(), 2);
  QCOMPARE(m2.effect_time(), 50);

  //augument
  m1.augument(m2);

  //validate
  QCOMPARE(m1.creature_stats().get_attribute(Attribute::IMP), 4);
  QCOMPARE(m1.creature_stats().get_attribute(Attribute::STR), 3);
  QCOMPARE(m1.creature_stats().get_skill(Skill::Gornictwo), 34);
  QCOMPARE(m1.creature_stats().get_skill(Skill::Odpornosc), 4);
  QCOMPARE(m1.global_test_level_mod(), 3);
  QCOMPARE(m1.effect_time(), -1);

  //remove augument
  m1.remove_augument(m2);

  QCOMPARE(m1.creature_stats().get_attribute(Attribute::IMP), 3);
  QCOMPARE(m1.creature_stats().get_attribute(Attribute::STR), 0);
  QCOMPARE(m1.creature_stats().get_skill(Skill::Gornictwo), 34);
  QCOMPARE(m1.creature_stats().get_skill(Skill::Odpornosc), 0);
  QCOMPARE(m1.global_test_level_mod(), 1);
  QCOMPARE(m1.effect_time(), -1);
}

void TestCreature::modmanager()
{
  //create manager
  CreatureModificatorManager manager;
  //create some mods
  shared_ptr<CreatureModificator> mod1(new CreatureModificator), mod2(new CreatureModificator), mod3(new CreatureModificator);

  //set the mods
  mod1->creature_stats().set_attribute(Attribute::CHR, 1);
  mod1->creature_stats().set_attribute(Attribute::STR, 1);
  mod1->save_to_db();

  mod2->creature_stats().set_attribute(Attribute::CHR, 2);
  mod2->creature_stats().set_attribute(Attribute::DEX, 1);
  mod2->save_to_db();

  mod3->creature_stats().set_attribute(Attribute::CHR, 3);
  mod3->creature_stats().set_attribute(Attribute::IMP, 1);
  mod3->save_to_db();

  //ADD mods to manager
  manager.add(mod1);
  manager.add(mod2);
  manager.add(mod3);

  //validate size
  QCOMPARE(manager.get_all().size(), (size_t)3);
  //validate complex mod
  QCOMPARE(manager.get_complex_mod().creature_stats().get_attribute(Attribute::CHR), 6);
  QCOMPARE(manager.get_complex_mod().creature_stats().get_attribute(Attribute::STR), 1);
  QCOMPARE(manager.get_complex_mod().creature_stats().get_attribute(Attribute::DEX), 1);
  QCOMPARE(manager.get_complex_mod().creature_stats().get_attribute(Attribute::IMP), 1);

  //REMOVE mod2
  manager.remove(mod2->ref());

  //validate size
  QCOMPARE(manager.get_all().size(), (size_t)2);
  //validate complex mod
  QCOMPARE(manager.get_complex_mod().creature_stats().get_attribute(Attribute::CHR), 4);
  QCOMPARE(manager.get_complex_mod().creature_stats().get_attribute(Attribute::STR), 1);
  QCOMPARE(manager.get_complex_mod().creature_stats().get_attribute(Attribute::DEX), 0);
  QCOMPARE(manager.get_complex_mod().creature_stats().get_attribute(Attribute::IMP), 1);

  mod1->purge();
  mod2->purge();
  mod3->purge();
}

void TestCreature::modmanager_ticktime()
{
  //create manager
  CreatureModificatorManager manager;
  //create some mods
  shared_ptr<CreatureModificator> mod1(new CreatureModificator), mod2(new CreatureModificator),
                                  mod3(new CreatureModificator), mod4(new CreatureModificator);
  //set mods time
  mod1->set_effect_time(10);
  mod1->save_to_db();
  mod2->set_effect_time(10);
  mod2->save_to_db();
  mod3->set_effect_time(20);
  mod3->save_to_db();
  mod4->save_to_db();

  //ADD mods to manager
  manager.add(mod1);
  manager.add(mod2);
  manager.add(mod3);
  manager.add(mod4);

  //validate size
  QCOMPARE(manager.get_all().size(), (size_t)4);

  //TICK 4
  GameClock::Clock().tick_time(4);

  //validate size
  QCOMPARE(manager.get_all().size(), (size_t)4);

  //TICK 10
  GameClock::Clock().tick_time(10);

  //validate size
  QCOMPARE(manager.get_all().size(), (size_t)2);

  //TICK 6
  GameClock::Clock().tick_time(6);

  //validate size
  QCOMPARE(manager.get_all().size(), (size_t)1);

  mod1->purge();
  mod2->purge();
  mod3->purge();
  mod4->purge();
}

void TestCreature::creature_creation()
{
  unique_ptr<Creature> crt = Creature::prototypes().clone(CreaturePrototype::Goblin);
  dbRef ref = crt->ref();
  QVERIFY(crt->ref() != 0);

  delete crt.release();
  crt = Creature::create(ref);
  QVERIFY(crt->ref() == ref);

  crt->purge();
}

void TestCreature::creature_load_base()
{
  unique_ptr<Creature> crt = Creature::prototypes().clone(CreaturePrototype::Goblin);

  QVERIFY(crt->ref() != 0);
  QCOMPARE(crt->name().c_str(), "Goblin");
  QCOMPARE(crt->descript().c_str(), "Goblin zielony taki");
  QVERIFY(crt->sex() == Sex::Male);

  crt->purge();
}

void TestCreature::creature_save_base()
{
  unique_ptr<Creature> crt = Creature::prototypes().clone(CreaturePrototype::Goblin);
  dbRef ref = crt->ref();

  crt->set_name("lol");
  crt->set_descript("lal");
  crt->set_sex(Sex::Female);

  QCOMPARE(crt->name().c_str(), "lol");
  QCOMPARE(crt->descript().c_str(), "lal");
  QVERIFY(crt->sex() == Sex::Female);

  //reset
  delete crt.release();
  crt = Creature::create(ref);

  //validate
  QCOMPARE(crt->name().c_str(), "lol");
  QCOMPARE(crt->descript().c_str(), "lal");
  QVERIFY(crt->sex() == Sex::Female);

  crt->purge();
}

void TestCreature::creature_load_save_stats()
{
  //create some creature
  unique_ptr<Creature> crt = Creature::prototypes().clone(CreaturePrototype::Goblin);
  dbRef ref = crt->ref();

  //set stats
  crt->set_skill(Skill::Akrobatyka, 4);
  crt->set_attribute(Attribute::DEX, 7);

  //reset item
  delete crt.release();
  crt = Creature::create(ref);

  //validate load
  QCOMPARE(crt->get_skill(Skill::Akrobatyka), 4);
  QCOMPARE(crt->get_attribute(Attribute::DEX), 7);
  crt->purge();
}

void TestCreature::creature_load_save_body()
{
  //create some creature
  unique_ptr<Creature> crt = Creature::prototypes().clone(CreaturePrototype::Goblin);
  Creature *crtp = crt.get();
  dbRef ref = crt->ref();

  //create some body parts
  BodyPart *bp1 = new BodyPart;
  bp1->set_type(BodyPartType::Glowa);
  bp1->set_region(BodyRegion::Gora);

  BodyPart *bp2 = new BodyPart;
  bp2->set_type(BodyPartType::Reka);
  bp2->set_region(BodyRegion::Gora);

  BodyPart *bp3 = new BodyPart;
  bp3->set_type(BodyPartType::Noga);
  bp3->set_region(BodyRegion::Dol);

  //dodaj party do kriczera
  crtp->_body.push_back(shared_ptr<BodyPart>(bp1));
  crtp->_body.push_back(shared_ptr<BodyPart>(bp2));
  crtp->_body.push_back(shared_ptr<BodyPart>(bp3));

  QCOMPARE(crtp->body_parts().size(), (size_t)3);

  //reset
  delete crt.release();
  crt = Creature::create(ref);
  crtp = crt.get();

  //validate load
  QCOMPARE(crtp->body_parts().size(), (size_t)3);
  crtp->purge();
}

void TestCreature::creature_load_inventory()
{
  //create some creature
  unique_ptr<Creature> crt = Creature::prototypes().clone(CreaturePrototype::Goblin);
  Creature *crtp = crt.get();
  dbRef ref = crt->ref();

  //create some item
  shared_ptr<Item> item1( Item::prototypes().clone(ItemPrototype::Noz).release() );

  //insert item
  crtp->take(item1);

  delete crt.release();
  crt = Creature::create(ref);
  crtp = crt.get();

  QCOMPARE(crtp->_inventory->get_all().size(), (size_t)1);

  crtp->purge();
}

void TestCreature::creature_load_modificators()
{
//  //create some creature
//  //a jebne szarda bo ten się nei jebie jak junik z podpowiedziami, a robienie plain pojntera to tez myli lol
//  shared_ptr<Creature> crt (Creature::prototypes().clone(CreaturePrototype::Goblin).release());
//  dbRef ref = crt->ref();

//  //dobra to teraz wciepac trzeba kilka modsów
//  //1. mod z założonego itema!
//  shared_ptr<CreatureModificator> i_mod1(new CreatureModificator);
//  i_mod1->creature_stats().set_attribute(Attribute::STR, 1);
//  shared_ptr<CreatureModificator> i_mod2(new CreatureModificator);
//  i_mod2->creature_stats().set_attribute(Attribute::STR, 2);

//  shared_ptr<Item> item1( Item::prototypes().clone(ItemPrototype::Noz).release() );
//  item1->mods().add(i_mod1);
//  item1->mods().add(i_mod2); //nożyk STR+3 oł je

//  crt->equip(item1);

//  //2. Wciapanie modów niezależnych
//  shared_ptr<CreatureModificator> n_mod1(new CreatureModificator);
//  shared_ptr<CreatureModificator> n_mod2(new CreatureModificator);
//  n_mod1->creature_stats().set_skill(Skill::Walka_Miecze, 10);
//  n_mod2->creature_stats().set_skill(Skill::Walka_Topory, 6);

//  crt->mods().add(n_mod1);
//  crt->mods().add(n_mod2);

}

