#include "db.h"

using namespace std;
using namespace soci;

const string DB::_db_file = "/home/pi/db/data.fdb";
const string DB::_db_log_file = "../amarlon/Data/db.log";
const string DB::_db_server = "lonski.pl";//"192.168.1.5";

//===DB Object
void DBObject::save_to_db()
{
  if (!isTemporary())
  {
    try
    {
      _Database.begin();

      for (auto i = _save_queries.begin(); i != _save_queries.end(); ++i)
      {
        if (*i != "")
        {
          _Database << *i;
        }
      }

      _Database.commit();
    }
    catch(soci_error &e)
    {
      qDebug() << "###Error saving " << table().c_str() << " ref=" << ref() << ": ";
      qDebug() << e.what();
      qDebug() << _Database.get_last_query().c_str();
    }

    _save_queries.clear();
  }
}

void DBObject::reload()
{
  save_to_db();
  set_not_loaded();
  load();
}

void DBObject::purge()
{
  if ( !isTemporary() )
  {
    _save_queries.clear();
    _Database << "delete from " << table().c_str() << " where ref = " << ref();
    _Database.commit();
    _temporary = true;
  }
}

void DBObject::save(string query)
{
  if ( loaded() && !isTemporary() ) _save_queries.push_back(query);
}

DBObject::~DBObject()
{
  if ( !isTemporary() )
  {
    try
    {
      save_to_db();
    }
    catch(std::exception &e)
    {
      qDebug() << "Error saving DBObject " << ref() << " : " << e.what();
    }
    catch(...)
    {
      qDebug() << "Error saving DBObject " << ref() << ".";
    }
  }
}

//===~~~

//===Database connection
DB* DB::_instance = nullptr;

DB::DB()
{
  _session = new soci::session;
  _session->open(*soci::factory_firebird(), "service="+_db_server+":/"+_db_file+" user=SYSDBA password=sl1wkowe");

  _log_stream = new ofstream(_db_log_file, ios::app);
  _session->set_log_stream(_log_stream);

}

DB *DB::Instance()
{
  if (_instance == nullptr)  {
    _instance = new DB;
  }

  return _instance;
}

soci::session& DB::session()
{
  *_log_stream << flush;
  return *_session;
}

soci::session &DB::Session()
{
  return DB::Instance()->session();
}

DB::~DB()
{
  _log_stream->close();
  delete _session;
  delete _log_stream;
}

//===~~~
