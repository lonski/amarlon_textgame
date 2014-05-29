#ifndef COMMON_H
#define COMMON_H

#include "inc.h"
#include "enums.h"

#define _Database DB::Session()
typedef unsigned int Ref;

//===DB Object
class DBObject{
  Ref _ref;
public:
  DBObject(Ref ref): _ref(ref) {}
  Ref ref() const { return _ref; }
  virtual ~DBObject() {}
};
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
