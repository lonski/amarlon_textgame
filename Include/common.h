#ifndef COMMON_H
#define COMMON_H

#include "inc.h"

#define __sql DB::Session()
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

#endif // COMMON_H
