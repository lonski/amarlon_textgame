#include "take.h"

Take::Take()
{
  add_name("podnies");
  add_name("podnieś");
  add_name("wez");
  add_name("weź");
  add_name("zabierz");
}

CommandID Take::id() const
{
  return CommandID::Take;
}

void Take::execute(std::vector<std::string> params)
{
  //TODO
}