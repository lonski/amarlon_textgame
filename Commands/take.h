#ifndef TAKE_H
#define TAKE_H

#include "command.h"

class Take : public NonActiveCommand
{
public:
  Take();
  virtual ~Take() {}

  virtual CommandID id() const;
  virtual void execute(std::vector<std::string>);
};

#endif // TAKE_H
