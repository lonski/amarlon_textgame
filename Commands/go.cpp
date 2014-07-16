#include "go.h"

Go::Go()
{
  add_name("n");
  add_name("s");
  add_name("w");
  add_name("e");
}

CommandID Go::id() const
{
  return CommandID::Go;
}

void Go::execute(std::vector<std::string> params)
{
  //TODO
}
