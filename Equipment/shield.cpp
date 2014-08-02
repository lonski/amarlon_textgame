#include "shield.h"

using namespace std;
using namespace soci;
using namespace fun;

Shield::Shield(dbRef ref, bool temporary): Item(ref, temporary)
{
}

void Shield::load(MapRow *data_source)
{
  if ( !loaded() && ref() > 0 ){
    try
    {
      MapRow item_data;
      if (data_source != nullptr)
      {
        item_data = *data_source;
      }
      else
      {
        item_data = MapQuery( "SELECT * FROM "+table()+" WHERE ref="+toStr(ref()) );
      }

      if (!item_data.empty())
      {
        set_defence(CheckField<int>(item_data["SHD_DEFENCE"]));
      }

      Item::load(&item_data);
      set_not_modified();
    }
    catch(soci_error &e)
    {
      MsgError(e.what());
      qDebug() << _Database.get_last_query().c_str();
    }
  }
}

void Shield::saveToDB()
{
  stringstream save_query;

  save_query << "UPDATE " << table() << " SET"
             << " SHD_DEFENCE=" << _defence
             << " WHERE ref=" << ref();

  save(save_query.str());
  Item::saveToDB();
}

void Shield::set_defence(int defence)
{
  _defence = defence;
  set_modified();
}

Shield::~Shield()
{
  _saveToDB_
}
