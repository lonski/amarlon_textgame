#ifndef PROTOTYPEMANAGER_H
#define PROTOTYPEMANAGER_H

#include "Include/inc.h"
#include "Include/db.h"
#include "Include/func.h"

//==PrototypeManager declaration
template<class T, typename EnumT>
class PrototypeManager
{
private:
  std::map<EnumT, std::unique_ptr<T> > _prototypes;
  void load_all();
public:
  PrototypeManager();
  std::unique_ptr<T> clone(EnumT key);
  size_t count();
  void reload();
};
//~~~

//==PrototypeManager definition
template<class T, typename EnumT>
PrototypeManager<T,EnumT>::PrototypeManager()
{
  load_all();
}

template<class T, typename EnumT>
std::unique_ptr<T> PrototypeManager<T,EnumT>::clone(EnumT key)
{
  std::unique_ptr<T> new_t(nullptr);
  auto found = _prototypes.find(key);

  if (found != _prototypes.end())
  {
    new_t = found->second->clone();
  }

  return new_t;
}

template<class T, typename EnumT>
void PrototypeManager<T,EnumT>::load_all()
{
  MapTable protos;
  fun::MapQuery("SELECT ref, prototype FROM "+T::table_name+" WHERE obj_type="+fun::toStr(static_cast<int>(ObjType::Prototype)), protos);

  //for each connection
  for (auto p = protos.begin(); p != protos.end(); ++p)
  {
    dbRef ref = fun::CheckField<dbRef>( (*p)["REF"] );
    EnumT type = fun::CheckFieldCast<EnumT>( (*p)["PROTOTYPE"] );

    _prototypes[type] = T::create(ref, true);
  }

  _Database.commit();
}

template<class T, typename EnumT>
size_t PrototypeManager<T,EnumT>::count()
{
  return _prototypes.size();
}

template<class T, typename EnumT>
void PrototypeManager<T,EnumT>::reload()
{
  _prototypes.clear();
  load_all();
}

//~~~

#endif // PROTOTYPEMANAGER_H
