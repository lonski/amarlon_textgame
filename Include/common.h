#ifndef COMMON_H
#define COMMON_H

#include "inc.h"
#include "enums.h"

#define _Database DB::Session()
typedef unsigned int dbRef;
typedef std::string dbTable;

//===DB Object
class DBObject{
private:
  dbRef _ref;
  dbTable _table;
  bool _loaded;
  std::list<std::string> _save_queries;
protected:
  //save changes
  template<typename T> void save(std::string f_name, T f_val, dbTable tbl = "");
  virtual void save(std::string query);
  virtual void set_loaded() { _loaded = true; }
  virtual void set_not_loaded() { _loaded = false; }

public:
  //birth and death
  DBObject(dbRef ref, dbTable table): _ref(ref), _table(table), _loaded(false) {}
  virtual ~DBObject() = 0;

  //data access
  virtual dbRef ref() const { return _ref; }
  virtual dbTable table() const { return _table; }
  virtual bool loaded() const { return _loaded; }

  //operations
  virtual void save_to_db();
  virtual void load() = 0;

};

template<typename T>
void DBObject::save(std::string f_name, T f_val, dbTable tbl)
{
  if ( ref() && loaded() )
  {
    std::stringstream s;
    s << "UPDATE "+ ( tbl == "" ? _table : tbl) + " SET " << f_name << "=\'"<<f_val<<"\' WHERE ref="<<ref();
    _save_queries.push_back(s.str());
    }
}

//===~~~

//===Database connection
class DB {
  const static std::string _db_file;
  const static std::string _db_log_file;
  const static std::string _db_server;
  static DB *_instance;

  soci::session *_session;
  std::ofstream *_log_stream;

  soci::session& session();

  DB();
public:
  static DB* Instance();
  static soci::session& Session();
  ~DB();
};
//===~~~

//===WalkVector (used in Location to walk within range)
class WalkVector{
private:
  short _vals[4];
  short indexof(Directions dir){
    switch(dir){
      case Directions::North: return 0; break;
      case Directions::South: return 1; break;
      case Directions::East: return 2; break;
      case Directions::West: return 3; break;
      default : return -1; break;
    }
  }

public:
  WalkVector(short north = 0, short south = 0, short east = 0, short west = 0 )
  : _vals{north, south, east, west}
  {
  }
  void inc(Directions dir, short val = 1)
  {
    short i = indexof(dir);
    if (i != -1) _vals[indexof(dir)] += val;
  }
  void dec(Directions dir, short val = 1)
  {
    short i = indexof(dir);
    if (i != -1) _vals[indexof(dir)] -= val;
  }
  short operator[](Directions dir)
  {
    short i = indexof(dir);
    return ( i != -1 ? _vals[i] : 0);
  }
};
//===~~

#endif // COMMON_H
