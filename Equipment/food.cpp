#include "food.h"

using namespace std;
using namespace soci;
using namespace fun;

Food::Food(dbRef ref, bool temporary): Item(ref, temporary)
{
}

void Food::load(MapRow *data_source)
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
        set_hunger(CheckField<int>(item_data["FOD_HUNGER"]));
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

void Food::saveToDB()
{
  stringstream save_query;

  save_query << "UPDATE " << table() << " SET"
             << " FOD_HUNGER=" << _hunger
             << " WHERE ref=" << ref();

  save(save_query.str());
  Item::saveToDB();
}

void Food::set_hunger(int hunger)
{
  _hunger = hunger;
  set_modified();
}

Food::~Food()
{
  _saveToDB_
}
