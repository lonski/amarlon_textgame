#include "common.h"

using namespace std;

//===Database connection

DB* DB::_instance = nullptr;
const string DB::_db_file = "/home/spszenguo/Projects/Amarlon/Data/data.fdb";
const string DB::_db_log_file = "/home/spszenguo/Projects/Amarlon/Data/db.log";

DB::DB()
{
  _session = new soci::session;
  _session->open(*soci::factory_firebird(), "service="+_db_file+" user=SYSDBA password=sl1wkowe");  

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
