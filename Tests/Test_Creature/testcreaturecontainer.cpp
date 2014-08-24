#include "testcreaturecontainer.h"
#include "../Creatures/creaturecontainer.h"

using namespace std;

TestCreatureContainer::TestCreatureContainer()
{
}

void TestCreatureContainer::creature_container_creation()
{
  Creature::Container *cont = new Creature::Container;
  dbRef ref = cont->ref();
  QVERIFY(ref != 0);

  //delete and use other constructor
  delete cont;
  cont = new Creature::Container(ref);

  QCOMPARE(cont->ref(), ref);

  cont->purge();
  delete cont;
}

void TestCreatureContainer::creature_container_load_save()
{
  Creature::Container *cont = new Creature::Container;
  dbRef ref = cont->ref();
  cont->setORef(2);
  cont->setOTable("lol");

  QCOMPARE(cont->oref(), (dbRef)2);
  QCOMPARE(cont->otable().c_str(), "lol");

  //recreate and check
  delete cont;
  cont = new Creature::Container(ref);

  QCOMPARE(cont->oref(), (dbRef)2);
  QCOMPARE(cont->otable().c_str(), "lol");

  cont->purge();
  delete cont;
}

void TestCreatureContainer::creature_container_insert_erase()
{
  Creature::Container *cont = new Creature::Container;
  dbRef cont_ref = cont->ref();

  //create some kriczers
  shared_ptr<Creature> ogr1 ( Creature::prototypes().clone(CreaturePrototype::Ogr) );
  dbRef ogr1_ref = ogr1->ref();
  ogr1->setName("Karol");

  shared_ptr<Creature> ogr2 ( Creature::prototypes().clone(CreaturePrototype::Ogr) );
  dbRef ogr2_ref = ogr2->ref();
  ogr2->setName("Maciek");

  //=========insert
  cont->insert(ogr1);
  cont->insert(ogr2);

  //validate
  QCOMPARE(cont->getAll().size(), (size_t)2);
  QVERIFY(cont->find(ogr1_ref) != nullptr);
  QVERIFY(cont->find(ogr2_ref) != nullptr);

  //RELOAD AND VALIDATE
  delete cont;
  ogr1.reset();
  ogr2.reset();

  cont = new Creature::Container(cont_ref);

  QCOMPARE(cont->getAll().size(), (size_t)2);
  QVERIFY(cont->find(ogr1_ref) != nullptr);
  QVERIFY(cont->find(ogr2_ref) != nullptr);

  //==========erase
  cont->erase(ogr2_ref);

  QCOMPARE(cont->getAll().size(), (size_t)1);
  QVERIFY(cont->find(ogr1_ref) != nullptr);
  QVERIFY(cont->find(ogr2_ref) == nullptr);

  //RELOAD AND VALIDATE
  delete cont;
  cont = new Creature::Container(cont_ref);

  QCOMPARE(cont->getAll().size(), (size_t)1);
  QVERIFY(cont->find(ogr1_ref) != nullptr);
  QVERIFY(cont->find(ogr2_ref) == nullptr);

  ogr2.reset(Creature::create(ogr2_ref));
  ogr1.reset(Creature::create(ogr1_ref));
  ogr2->purge();
  ogr1->purge();
  cont->purge();
}
