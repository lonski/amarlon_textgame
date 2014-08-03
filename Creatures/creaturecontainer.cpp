#include "creaturecontainer.h"

using namespace std;

dbRef Creature::Container::byOwner(dbTable otable, dbRef oref)
{
  dbRef ref = 0;
  soci::indicator ind;
  _Database << "SELECT ref FROM crt_containers WHERE otable='"<<otable<<"' and oref="<<fun::toStr(oref), into(ref, ind);
  if (ind != soci::i_ok) ref = 0;

  return ref;
}

Creature::Container::Container(dbRef ref)
  : DBObject(ref)
{
  load();
}

Creature::Container::Container()
  : DBObject(0)
  , _otable("")
  , _oref(0)
{
  dbRef ref = 0;
  soci::indicator ind;
  _Database << "SELECT new_ref FROM create_crt_container", into(ref, ind);
  _Database.commit();
  if (ind != soci::i_ok) ref = 0;

  DBObject::set_ref(ref);
  DBObject::set_loaded();
  DBObject::set_not_modified();
}

Creature::Container::~Container()
{
  _saveToDB_
}

void Creature::Container::load(MapRow *data_source)
{
  if ( !loaded() && ref() > 0 )
  {
    try
    {
      MapRow cont_data;
      if (data_source != nullptr)
      {
        cont_data = *data_source;
      }
      else
      {
        cont_data = fun::MapQuery("SELECT otable, oref, creatures FROM "+tableName+" WHERE ref="+fun::toStr(ref()));
      }

      //==header data
      if (!cont_data.empty())
      {
        setORef( fun::CheckField<dbRef>(cont_data["OREF"]) );
        setOTable( fun::CheckField<std::string>(cont_data["OTABLE"]) );
        str2creatures( fun::CheckField<std::string>(cont_data["CREATURES"]) );
      }

      set_loaded();
      set_not_modified();
    }
    catch(soci::soci_error &e)
    {
      fun::MsgError(e.what());
      qDebug() << _Database.get_last_query().c_str();
    }
    catch(std::exception &e)
    {
      fun::MsgError(e.what());
    }
  }
}

void Creature::Container::saveToDB()
{
  stringstream save_query;

  save_query << "UPDATE " << table() << " SET"
             << "  CREATURES='" << creatures2str() << "'"
             << " ,OTABLE='" << _otable << "'"
             << " ,OREF=" << _oref
             << " WHERE ref=" << ref();

  save(save_query.str());
  DBObject::saveToDB();
}

void Creature::Container::insert(std::shared_ptr<Creature> &crt)
{
  if (_creatures.find(crt->ref()) == _creatures.end())
  {
    _creatures[crt->ref()] = crt;
    set_modified();
  }
  else
  {
    throw error::container_insertion_error("Istota już znajduje się w kontenerze.");
  }
}

std::shared_ptr<Creature> Creature::Container::erase(dbRef crt_ref)
{
  shared_ptr<Creature> r(nullptr);
  auto crt_iter = _creatures.find(crt_ref);

  if (crt_iter != _creatures.end())
  {
    r = crt_iter->second;
    _creatures.erase(crt_iter);
    set_modified();
  }

  return r;
}

std::shared_ptr<Creature> Creature::Container::find(dbRef crt_ref)
{
  shared_ptr<Creature> r(nullptr);
  auto crt_iter = _creatures.find(crt_ref);

  if (crt_iter != _creatures.end())
  {
    r = crt_iter->second;
  }

  return r;
}

std::vector<std::shared_ptr<Creature> > Creature::Container::getAll()
{
  vector<shared_ptr<Creature> > r;

  for (auto c = _creatures.begin(); c != _creatures.end(); ++c)
  {
    r.push_back(c->second);
  }

  return r;
}

void Creature::Container::setOTable(dbTable otable)
{
  _otable = otable;
  set_modified();
}

void Creature::Container::setORef(dbRef oref)
{
  _oref = oref;
  set_modified();
}


void Creature::Container::str2creatures(string crts)
{
  _creatures.clear();
  vector<string> crts_refs = fun::explode(crts,',');
  for (auto c = crts_refs.begin(); c != crts_refs.end(); ++c)
  {
    dbRef ref = fun::fromStr<dbRef>(*c);
    if (ref != 0)
    {
      _creatures[ref] = CreaturePtr(Creature::create(ref));
    }
  }
}

string Creature::Container::creatures2str()
{
  string r("");
  for (auto c = _creatures.begin(); c != _creatures.end(); ++c)
  {
    dbRef ref = c->second->ref();
    if (ref > 0)
    {
      r += fun::toStr(ref) + ",";
    }
  }
  return r;
}
