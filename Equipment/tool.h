#ifndef TOOL_H
#define TOOL_H

#include "item.h"

//==Tool
class Tool : public Item
{
private:
  friend class Item;
  Tool(dbRef ref);
public:
  virtual ~Tool() {}
};
//===

#endif // TOOL_H
