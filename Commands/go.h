#ifndef GO_H
#define GO_H

#include "command.h"

class Go : public NonActiveCommand
{
public:  
  Go();
  virtual ~Go() {}

  virtual CommandID id() const;
  virtual void execute(std::vector<std::string> params);
  virtual void execute(std::string = "");
};

#endif // GO_H
