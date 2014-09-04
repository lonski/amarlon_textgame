#include "testcreature.h"
#include "Equipment/item_container.h"
#include "Creatures/body.h"

using namespace std;

void TestCreature::creature_creation()
{
  unique_ptr<Creature> crt  (Creature::prototypes().clone(CreaturePrototype::Ogr));

  dbRef ref = crt->ref();
  QVERIFY(crt->ref() != 0);

  delete crt.release();
  crt.reset(Creature::create(ref));
  QVERIFY(crt->ref() == ref);

  crt->purge();
}

void TestCreature::creature_load_base()
{
  unique_ptr<Creature> crt (Creature::prototypes().clone(CreaturePrototype::Ogr));

  QVERIFY(crt->ref() != 0);
  QCOMPARE(crt->name().c_str(), "Ogr");
  QCOMPARE(crt->descript().c_str(), "");
  QVERIFY(crt->sex() == Sex::Male);

  crt->purge();
}

void TestCreature::creature_save_base()
{
  unique_ptr<Creature> crt (Creature::prototypes().clone(CreaturePrototype::Ogr));
  dbRef ref = crt->ref();

  crt->setName("lol");
  crt->setDescript("lal");
  crt->setSex(Sex::Female);

  QCOMPARE(crt->name().c_str(), "lol");
  QCOMPARE(crt->descript().c_str(), "lal");
  QVERIFY(crt->sex() == Sex::Female);

  //reset
  delete crt.release();
  crt.reset( Creature::create(ref) );

  //validate
  QCOMPARE(crt->name().c_str(), "lol");
  QCOMPARE(crt->descript().c_str(), "lal");
  QVERIFY(crt->sex() == Sex::Female);

  crt->purge();
}

void TestCreature::creature_load_save_stats()
{
  //create some creature
  unique_ptr<Creature> crt (Creature::prototypes().clone(CreaturePrototype::Ogr));
  dbRef ref = crt->ref();

  //set stats
  crt->setSkill(Skill::Akrobatyka, 4);
  crt->setAttribute(Attribute::DEX, 7);

  //reset item
  delete crt.release();
  crt.reset( Creature::create(ref) );

  //validate load
  QCOMPARE(crt->skill(Skill::Akrobatyka), 4);
  QCOMPARE(crt->attribute(Attribute::DEX), 7);
  crt->purge();
}

void TestCreature::creature_load_save_body()
{
  //create some creature
  unique_ptr<Creature> crt (Creature::prototypes().clone(CreaturePrototype::Ogr));
  Creature *crtp = crt.get();
  dbRef ref = crt->ref();

  //create some body parts
  BodyPart *bp1 = new BodyPart;
  bp1->setType(BodyPartType::Glowa);
  bp1->setRegion(BodyRegion::Gora);

  BodyPart *bp2 = new BodyPart;
  bp2->setType(BodyPartType::Reka);
  bp2->setRegion(BodyRegion::Gora);

  BodyPart *bp3 = new BodyPart;
  bp3->setType(BodyPartType::Noga);
  bp3->setRegion(BodyRegion::Dol);

  //dodaj party do kriczera
  crtp->body().load(bp1->toStr()+";"+bp2->toStr()+";"+bp3->toStr()+";");

  QCOMPARE(crtp->body().parts().size(), (size_t)3);

  //reset
  delete crt.release();
  crt.reset( Creature::create(ref) );
  crtp = crt.get();

  //validate load
  QCOMPARE(crtp->body().parts().size(), (size_t)3);
  crtp->purge();
}

void TestCreature::creature_load_inventory()
{
  //create some creature
  unique_ptr<Creature> crt (Creature::prototypes().clone(CreaturePrototype::Ogr));
  Creature *crtp = crt.get();
  dbRef ref = crt->ref();

  //create some item
  ItemPtr item1( Item::prototypes().clone(ItemPrototype::Noz) );

  //insert item
  crtp->take(item1);

  QVERIFY(crtp->inventoryContainer() != nullptr);
  QCOMPARE(crtp->inventoryContainer()->getAll().size(), (size_t)1);

  delete crt.release();
  crt.reset( Creature::create(ref) );
  crtp = crt.get();

  QVERIFY(crtp->inventoryContainer() != nullptr);
  QCOMPARE(crtp->inventoryContainer()->getAll().size(), (size_t)1);

  auto items = crtp->inventoryContainer()->getAll();
  for (auto i = items.begin(); i != items.end(); ++i)
    i->item->purge();

  crtp->purge();
}

void TestCreature::creature_load_modificators()
{
  //create some creature
  //a jebne szarda bo ten się nei jebie jak junik z podpowiedziami, a robienie plain pojntera to tez myli lol
  shared_ptr<Creature> crt (Creature::prototypes().clone(CreaturePrototype::Ogr));
  //dbRef ref = crt->ref();

  //bazowe staty bez mod
  int str = crt->attribute(Attribute::STR);
  int walka_miecze = crt->skill(Skill::Walka_Miecze);
  int walka_topory = crt->skill(Skill::Walka_Topory);
  //qDebug() << str << " " << walka_miecze << " " << walka_topory;
  qDebug() << "X";
  //dobra to teraz wciepac trzeba kilka modsów
  //1. mod z założonego itema!
  shared_ptr<CreatureModificator> i_mod1(new CreatureModificator);
  i_mod1->creature_stats().setAttribute(Attribute::STR, 1);
  i_mod1->saveToDB();
  shared_ptr<CreatureModificator> i_mod2(new CreatureModificator);
  i_mod2->creature_stats().setAttribute(Attribute::STR, 2);
  i_mod2->saveToDB();

  ItemPtr item1( Item::prototypes().clone(ItemPrototype::Noz) );
  item1->mods()->add(i_mod1);
  item1->mods()->add(i_mod2); //nożyk STR+3 oł je
  //item1->mods()->get_complex_mod()->saveToDB();

  QCOMPARE(item1->mods()->get_complex_mod()->creature_stats().attribute(Attribute::STR), 3);

  crt->equip(item1);

  //2. Wciapanie modów niezależnych
  shared_ptr<CreatureModificator> n_mod1(new CreatureModificator);
  shared_ptr<CreatureModificator> n_mod2(new CreatureModificator);
  n_mod1->creature_stats().setSkill(Skill::Walka_Miecze, 10);
  n_mod1->saveToDB();
  n_mod2->creature_stats().setSkill(Skill::Walka_Topory, 6);
  n_mod2->saveToDB();

  crt->mods()->add(n_mod1);
  crt->mods()->add(n_mod2);

  QCOMPARE( crt->skill(Skill::Walka_Miecze), walka_miecze+10 );
  QCOMPARE( crt->skill(Skill::Walka_Topory), walka_topory+6 );
  QCOMPARE( crt->attribute(Attribute::STR), str+3 );

  i_mod1->purge();
  i_mod2->purge();
  n_mod1->purge();
  n_mod2->purge();
  item1->mods()->get_complex_mod()->purge();
  item1->purge();  
  crt->purge();
}

void TestCreature::creature_eq()
{
  //Create some creature
  shared_ptr<Creature> crt ( Creature::prototypes().clone(CreaturePrototype::Ogr) );
  CreatureMonitor c_monit(crt.get());

  //create some equipment
  ItemPtr zbroja ( ( Item::prototypes().clone(ItemPrototype::LekkaZbrojaSkorzana) ) );
  ItemPtr zbroja_druga ( ( Item::prototypes().clone(ItemPrototype::LekkaZbrojaSkorzana) ) );
  ItemPtr helm ( ( Item::prototypes().clone(ItemPrototype::HelmSkorzany) ) );
  ItemPtr wpn ( Item::prototypes().clone(ItemPrototype::MieczSzeroki) );
  ItemPtr shld ( ( Item::prototypes().clone(ItemPrototype::MalaDrewnianaTarcza) ) );

  QVERIFY(zbroja != nullptr);
  QVERIFY(helm != nullptr);
  QVERIFY(wpn != nullptr);
  QVERIFY(shld != nullptr);

  try
  {
    crt->equip(zbroja);
    crt->equip(helm);
    crt->equip(wpn);
    crt->equip(shld);
  }
  catch(error::equip_no_bodyparts &e)
  {
    qDebug() << "ERR " << e.what();
    qDebug() << c_monit.print().c_str();

  }

  //CreatureMonitor monit(crt.get());
  //qDebug() << monit.print_mods().c_str();

  QCOMPARE(crt->mods()->get_complex_mod()->creature_stats().attribute(Attribute::DEX), -2 );
  QCOMPARE(crt->body().equipped_items().size(), (size_t)4 );

  shared_ptr<BodyPart> bp = crt->body().part(BodyPartType::Glowa, BodyRegion::Gora, BodySide::Center);
  QVERIFY(bp != nullptr);
  QVERIFY(bp->equipped(ItemType::Armor).lock() != nullptr);

  //reload
  dbRef ref = crt->ref();
  dbRef zbroja_ref = zbroja->ref();
  dbRef helm_ref = helm->ref();
  //wyjeb w pizdu
  zbroja.reset();
  helm.reset();
  wpn.reset();
  shld.reset();
  crt.reset();

  //stworz od nowa
  crt.reset(Creature::create(ref));

  c_monit.reset(crt.get());
  //qDebug() << c_monit.print().c_str();

  //sprawdz czy helm zalolzony
  bp = crt->body().part(BodyPartType::Glowa);
  QVERIFY(bp != nullptr);
  QVERIFY(bp->equipped(ItemType::Armor).lock() != nullptr);

  //sprawdz ilosc zalozonych itemów i zgodnosc modyfikatorów
  QCOMPARE(crt->body().equipped_items().size(), (size_t)4 );
  QCOMPARE(crt->mods()->get_complex_mod()->creature_stats().attribute(Attribute::DEX), -2 );

  //sciagnij zbroje, validate, i zaloz z powrotem
  zbroja = ( crt->unequip( zbroja_ref ) );
  QCOMPARE(crt->mods()->get_complex_mod()->creature_stats().attribute(Attribute::DEX), -1 );
  QCOMPARE(crt->body().equipped_items().size(), (size_t)3 );

  crt->equip(zbroja);
  QCOMPARE(crt->body().equipped_items().size(), (size_t)4 );
  QCOMPARE(crt->mods()->get_complex_mod()->creature_stats().attribute(Attribute::DEX), -2 );

  //sciagnij helm i wrzuc do ekw
  helm = ( crt->unequip(helm_ref) );
  crt->take(helm);

  QCOMPARE(crt->mods()->get_complex_mod()->creature_stats().attribute(Attribute::DEX), -2 );
  QCOMPARE(crt->body().equipped_items().size(), (size_t)3 );

  QCOMPARE(crt->inventory().size(), (size_t)1);
  QCOMPARE(crt->inventory().at(0).item->ref(), helm_ref );

  //spróboj założyć druga zbroję
  bool ok = false;
  try
  {
    crt->equip(zbroja_druga);
  }catch(error::equip_no_bodyparts)
  {
    ok = true;
  }
  QVERIFY(ok);
  QCOMPARE(crt->mods()->get_complex_mod()->creature_stats().attribute(Attribute::DEX), -2 );
  QCOMPARE(crt->body().equipped_items().size(), (size_t)3 );

  //clean all dis szit ap!
  zbroja_druga->purge();

  for (auto i = crt->body().equipped_items().begin(); i!=crt->body().equipped_items().end(); ++i)
    (*i)->purge();

  vector<AmountedItem > inv = crt->inventory();
  for (auto i = inv.begin(); i!=inv.end(); ++i)
    i->item->purge();

  crt->purge();

  /*STWORZENIE OGRA
  shared_ptr<MOB> ogr ( dynamic_cast<MOB*>(Creature::prototypes().clone(CreaturePrototype::BlankMOB).release()) );
  QVERIFY(ogr != nullptr);
  ogr->setName("Ogr");
  ogr->set_sex(Sex::Male);
  ogr->setAttribute(Attribute::STR,6);
  ogr->setAttribute(Attribute::DEX,3);
  ogr->setAttribute(Attribute::END,5);
  ogr->setAttribute(Attribute::REF,4);
  ogr->setAttribute(Attribute::INT,2);
  ogr->setAttribute(Attribute::CHR,3);
  ogr->setAttribute(Attribute::IMP,1);
  ogr->setAttribute(Attribute::WLL,5);
  ogr->setSkill(Skill::Walka_Topory, 6);

  BodyPart reka_l(BodyPartType::Reka, BodyRegion::Gora, BodySide::Left),
           reka_p(BodyPartType::Reka, BodyRegion::Gora, BodySide::Right),
           glowa(BodyPartType::Glowa, BodyRegion::Gora, BodySide::Center),
           tors(BodyPartType::Tors, BodyRegion::Gora, BodySide::Center),
           noga_l(BodyPartType::Noga, BodyRegion::Dol, BodySide::Left),
           noga_p(BodyPartType::Noga, BodyRegion::Dol, BodySide::Right);

  ogr->body().load(
    reka_l.toStr()+";"+
    reka_p.toStr()+";"+
    glowa.toStr()+";"+
    tors.toStr()+";"+
    noga_l.toStr()+";"+
    noga_p.toStr()+";"
  );
  */

  /*STWORZENIE ZBROI
  ArmorPtr zbroja ( dynamic_cast<Armor*>( Item::prototypes().clone(ItemPrototype::BlankArmor).release() ) );
  zbroja->setName("Lekka zbroja skórzana");
  zbroja->setDamageReduction(Damage(2,2,1));
  zbroja->setCondition(ItemCondition::Good);
  zbroja->setWeight(8);
  zbroja->setDurability(6);
  zbroja->setType(ItemType::Armor);
  zbroja->setStackable(false);
  zbroja->addBodyPart(BodyPartType::Tors);
  zbroja->addBodyPart(BodyPartType::Reka);
  zbroja->addBodyPart(BodyPartType::Reka);
  zbroja->addBodyPart(BodyPartType::Noga);
  zbroja->addBodyPart(BodyPartType::Noga);
  shared_ptr<CreatureModificator> zbroja_mod (new CreatureModificator);
  zbroja_mod->creature_stats().setAttribute(Attribute::DEX, -1);
  zbroja->mods()->add(zbroja_mod);
  */
  /*STWORZENIE TARCZY
  ShieldPtr sh ( dynamic_cast<Shield*>( Item::prototypes().clone(ItemPrototype::BlankShield).release() ) );
  sh->addBodyPart(BodyPartType::Reka);
  sh->setDefence(4);
  sh->setCondition(ItemCondition::Good);
  sh->setDurability(4);
  sh->setName("Mala drewniana tarcza");
  sh->setStackable(false);
  sh->setType(ItemType::Shield);
  sh->setWeight(1.7);
  shared_ptr<CreatureModificator> sh_mod (new CreatureModificator);
  sh_mod->creature_stats().setAttribute(Attribute::DEX, -1);
  sh->mods()->add(sh_mod);
  */
  /*STWORZENIE HELMU
  ArmorPtr helm ( dynamic_cast<Armor*>( Item::prototypes().clone(ItemPrototype::BlankArmor).release() ) );
  helm->setName("Skórzany hełm");
  helm->setDamageReduction(Damage(0,1,0));
  helm->setCondition(ItemCondition::Good);
  helm->setWeight(0.6);
  helm->setDurability(5);
  helm->setType(ItemType::Armor);
  helm->setStackable(false);
  helm->addBodyPart(BodyPartType::Glowa);
  */
  /*STWORZENIE BRONI
  WeaponPtr wpn ( dynamic_cast<Weapon*>( Item::prototypes().clone(ItemPrototype::BlankWeapon).release() ) );
  wpn->setAttack(2);
  wpn->setCondition(ItemCondition::Good);
  wpn->setDamage(Damage(4,5,0));
  wpn->setDefence(4);
  wpn->setDurability(9);
  wpn->setName("Miecz szeroki");
  wpn->setRange(0);
  wpn->setReflex(0);
  wpn->setSkill(WeaponSkill::Miecze);
  wpn->setStackable(false);
  wpn->setStrReq(3);
  wpn->setType(ItemType::Weapon);
  wpn->setWeight(1.8);
  */
}

void TestCreature::npc_load()
{
  shared_ptr<NPC> npc( dynamic_cast<NPC*>(Creature::prototypes().clone(CreaturePrototype::BlankNPC)) );
  dbRef ref = npc->ref();
  npc->setName("Igor");
  npc->set_clan(Clan::Aep_Tien);
  npc->set_tribe(Tribe::Tuatha_De_Cael);

  npc.reset();
  npc.reset( dynamic_cast<NPC*>(Creature::create(ref)) );

  QCOMPARE(npc->name().c_str(), "Igor");
  qDebug() << (int)npc->clan();
  QCOMPARE(npc->clan(), Clan::Aep_Tien);

  QCOMPARE(npc->tribe(), Tribe::Tuatha_De_Cael);

  npc->purge();
}

void TestCreature::player_base()
{
  QCOMPARE(_Player->ref(), static_cast<dbRef>(5));
  //QCOMPARE(_Player.name().c_str(), "Player");
}

void TestCreature::player_load()
{
  _Player->saveToDB();
}
