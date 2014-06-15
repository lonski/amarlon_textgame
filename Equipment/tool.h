#ifndef TOOL_H
#define TOOL_H

#include "item.h"

//==Tool
class Tool : public Item
{
private:
  friend class Item;
  Tool(dbRef ref, bool temporary = false);
public:
  virtual ~Tool() {}
};
//===

#endif // TOOL_H
