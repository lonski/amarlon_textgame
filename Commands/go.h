#ifndef GO_H
#define GO_H

#include "command.h"

class Go : public Command
{
public:
  Go();
  virtual ~Go() {}

  virtual CommandID id() const;
  virtual void execute(std::vector<std::string> params);
};

#endif // GO_H
