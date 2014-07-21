#ifndef DEBUG_H
#define DEBUG_H

#include "command.h"

class Debug : public NonActiveCommand
{
public:
  Debug();
  virtual ~Debug() {}

  virtual CommandID id() const;
  virtual void execute(std::vector<std::string> params);
};


#endif // DEBUG_H