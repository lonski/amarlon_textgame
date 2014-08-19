#include <QDebug>
#include <QMessageBox>
#include <fstream>
#include "soci-firebird.h"
#include "db.h"
#include <QDir>

using namespace std;
using namespace soci;

string DB::_db_file( "/home/pi/db/data.fdb");
string DB::_db_log_file( "db.log" );
string DB::_db_server = "192.168.1.5";

//===DB Object
void DBObject::saveToDB()
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
      qDebug() << "###Error saving ref=" << ref() << ": ";
      qDebug() << "#MSG = " << e.what();
      qDebug() << "#QUERY = " << _Database.get_last_query().c_str();
      QMessageBox::critical(NULL, "Error",_Database.get_last_query().c_str());
    }

    _save_queries.clear();
    set_not_modified();
  }
}

void DBObject::reload()
{
  saveToDB();
  set_not_loaded();
  load();
}

void DBObject::purge()
{
  if ( !isTemporary() && ref() > 0)
  {
    _save_queries.clear();
    _Database << "delete from " << table().c_str() << " where ref = " << ref();
    _Database.commit();
    _temporary = true;
  }
}

void DBObject::save(string query)
{
  if ( loaded() && !isTemporary() )
  {
    _save_queries.push_back(query);
  }
}

DBObject::~DBObject()
{
  if ( !isTemporary() && modified() )
  {
    try
    {
      saveToDB();
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
  : _session(nullptr)
{
}

DB *DB::Instance()
{
  if (_instance == nullptr)
  {
    _instance = new DB;    
    _instance->InitDatabase();
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

void DB::SetDatabaseInfo(string db_file, string db_server, string db_log)
{
  DB::_db_file = db_file;
  DB::_db_log_file = db_log;
  DB::_db_server = db_server;
}

void DB::InitDatabase()
{
  if (_session != nullptr)
  {
    _session->close();
  }
  else
  {
    _session = new soci::session;
  }

  _session->open(*soci::factory_firebird(), "service="+_db_server+":"+_db_file+" user=SYSDBA password=sl1wkowe");

  _log_stream = new ofstream(_db_log_file, ios::app);
  _session->set_log_stream(_log_stream);
}


DB::~DB()
{
  _log_stream->close();
  delete _session;
  delete _log_stream;
}

//===~~~


