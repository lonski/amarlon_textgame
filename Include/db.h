#ifndef COMMON_H
#define COMMON_H

#include "inc.h"
#include "enums.h"

#define _Database DB::Session()
typedef unsigned int dbRef;
typedef std::string dbTable;

//===DB Object
class DBObject
{
private:
  //data
  dbRef _ref;

  //flags
  bool _loaded;
  bool _modified;
  bool _temporary;

  //save queue
  std::list<std::string> _save_queries;

protected:
  //save changes
  template<typename T> void save(std::string f_name, T f_val, dbTable tbl = "");
  virtual void save(std::string query);
  virtual void set_loaded() { _loaded = true; }
  virtual void set_not_loaded() { _loaded = false; }
  virtual void set_modified() { _modified = true; }
  virtual void set_not_modified() { _modified = false; }

  //set data
  void set_ref(dbRef ref)
  {
    _ref = ref;
    set_not_loaded();
  }

public:
  //birth and death
  DBObject(dbRef ref, bool temporary = false): _ref(ref), _loaded(false), _modified(false), _temporary(temporary) {}
  virtual ~DBObject() = 0;

  //flags access
  bool isTemporary() const { return _temporary; }

  //data access
  virtual dbRef ref() const { return _ref; }
  virtual dbTable table() const = 0;
  virtual bool loaded() const { return _loaded; }
  virtual bool modified() const { return _modified; }

  //operations
  virtual void saveToDB();
  virtual void load(MapRow *data_source = nullptr) = 0;
  virtual void reload();
  virtual void purge();

};

template<typename T>
void DBObject::save(std::string f_name, T f_val, dbTable tbl)
{
  if ( ref() && loaded() && !isTemporary() )
  {
    std::stringstream s;
    s << "UPDATE "+ ( tbl == "" ? table() : tbl) + " SET " << f_name << "=\'"<<f_val<<"\' WHERE ref="<<ref();
    _save_queries.push_back(s.str());
  }
}

//===~~~

//===Database connection
class DB
{
private:
  static std::string _db_file;
  static std::string _db_log_file;
  static std::string _db_server;
  static DB *_instance;

  soci::session *_session;
  std::ofstream *_log_stream;

  soci::session& session();

  DB();
  DB(const DB&);
  DB& operator=(const DB&);
public:
  static DB* Instance();
  static soci::session& Session();
  static void SetDatabaseInfo(std::string db_file, std::string db_server, std::string db_log);
  void InitDatabase();
  ~DB();
};
//===~~~

#endif // COMMON_H
