#ifndef DB_UTILS_H
#define DB_UTILS_H

#include <string>
#include "Include/typedefs/def_db_table.h"

namespace fun
{

void MapQuery(const std::string &query, MapTable &result);
MapRow MapQuery(const std::string &query);

}

#endif // DB_UTILS_H
