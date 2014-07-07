#include "food.h"

using namespace std;
using namespace soci;
using namespace fun;

Food::Food(dbRef ref, bool temporary): Item(ref, temporary)
{
}

void Food::load()
{
  if ( !loaded() && ref() > 0 ){
    try
    {
      MapRow item_data = MapQuery( "SELECT * FROM "+table()+" WHERE ref="+toStr(ref()) );
      if (!item_data.empty())
      {
        set_hunger(CheckField<int>(item_data["FOD_HUNGER"]));
      }
    }
    catch(soci_error &e)
    {
      MsgError(e.what());
      qDebug() << _Database.get_last_query().c_str();
    }
  }

  Item::load();
}

void Food::set_hunger(int hunger)
{
  _hunger = hunger;
  save("FOD_HUNGER", _hunger);
}
