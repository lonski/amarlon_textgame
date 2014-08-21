#ifndef PROTOTYPEMANAGER_H
#define PROTOTYPEMANAGER_H

#include "Include/enums/e_objtype.h"
#include "Include/db.h"

#include "Include/exceptions.h"
#include "Include/functions/db_utils.h"
#include "Include/functions/string_utils.h"

//==PrototypeManager
template<class T, typename EnumT>
class PrototypeManager
{
private:
  std::map<EnumT, T* > _prototypes;
  void load_all();
public:
  PrototypeManager();
  T* clone(EnumT key);
  size_t count() const;
  void reload();
};

template<class T, typename EnumT>
PrototypeManager<T,EnumT>::PrototypeManager()
{
  load_all();
}

template<class T, typename EnumT>
T* PrototypeManager<T,EnumT>::clone(EnumT key)
{
  T* new_t(nullptr);
  auto found = _prototypes.find(key);

  if (found != _prototypes.end())
  {
    new_t = found->second->clone();
  }
  else
  {
    throw error::creation_error("Brak prototypu "+fun::toStr(static_cast<int>(key))+" dla tabeli "+T::tableName);
  }

  return new_t;
}

template<class T, typename EnumT>
void PrototypeManager<T,EnumT>::load_all()
{
  MapTable protos;
  fun::MapQuery("SELECT ref, prototype FROM "+T::tableName+" WHERE obj_type="+fun::toStr(static_cast<int>(ObjType::Prototype)), protos);

  for (auto p = protos.begin(); p != protos.end(); ++p)
  {    
    dbRef ref = fun::CheckValue<dbRef>( (*p)["REF"] );
    EnumT type = fun::CheckValueCast<EnumT>( (*p)["PROTOTYPE"] );

    _prototypes[type] = T::create(ref);
  }

  _Database.commit();
}

template<class T, typename EnumT>
size_t PrototypeManager<T,EnumT>::count() const
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

//==Prototypable
template<class T, typename EnumT>
class Prototypable
{
private:
  static PrototypeManager<T, EnumT> *_prototypes;
public:
  static PrototypeManager<T, EnumT>& prototypes();
  virtual T* clone() = 0;
};

template<class T, typename EnumT>
PrototypeManager<T, EnumT>& Prototypable<T, EnumT>::prototypes()
{
  if (_prototypes == nullptr) _prototypes = new PrototypeManager<T, EnumT>();
  return *_prototypes;
}

template<class T, typename EnumT>
PrototypeManager<T, EnumT>* Prototypable<T, EnumT>::_prototypes = nullptr;
//~~~

#endif // PROTOTYPEMANAGER_H
