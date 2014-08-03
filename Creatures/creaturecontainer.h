#ifndef CREATURECONTAINER_H
#define CREATURECONTAINER_H

#include "creature.h"

class Creature::Container : public DBObject
{
public:
  Container(dbRef ref);
  Container();
  ~Container();
  static dbRef byOwner(dbTable otable, dbRef oref);

  const static dbTable tableName;
  virtual dbTable table() const { return tableName; }

  virtual void load(MapRow *data_source = nullptr);
  virtual void saveToDB();

  void insert(std::shared_ptr<Creature>& crt);
  std::shared_ptr<Creature> erase(dbRef crt_ref);
  std::shared_ptr<Creature> find(dbRef crt_ref);
  std::vector<std::shared_ptr<Creature> > getAll();

  dbTable otable() const { return _otable; }
  dbRef oref() const { return _oref; }

  void setOTable(dbTable otable);
  void setORef(dbRef oref);

private:
  friend class TestCreature;

  dbTable _otable;
  dbRef _oref;

  std::map<dbRef, std::shared_ptr<Creature> > _creatures;
  void str2creatures(std::string crts);
  std::string creatures2str();

};

#endif // CREATURECONTAINER_H
