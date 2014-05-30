#ifndef PROTOTYPEMANAGER_H
#define PROTOTYPEMANAGER_H

#include "Include/inc.h"

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
  //TODO: Load prototypes from db
}
//~~~

#endif // PROTOTYPEMANAGER_H
