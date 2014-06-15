#include "shield.h"

using namespace std;
using namespace soci;
using namespace fun;

Shield::Shield(dbRef ref, bool temporary): Item(ref, temporary)
{
}

void Shield::load()
{
  if ( !loaded() && ref() > 0 ){
    try
    {
      MapRow item_data = MapQuery( "SELECT * FROM "+table()+" WHERE ref="+toStr(ref()) );
      if (item_data.size() > 0)
      {
        set_defence(CheckField<int>(item_data["SHD_DEFENCE"]));
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

void Shield::set_defence(int defence)
{
  _defence = defence;
  save("SHD_DEFENCE", _defence);
}
