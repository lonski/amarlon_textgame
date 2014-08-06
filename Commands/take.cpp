#include "take.h"

Take::Take(Console *console)
  : NonActiveCommand(console)
{
  addName("podnies");
  addName("podnieś");
  addName("wez");
  addName("weź");
  addName("zabierz");
}

CommandID Take::id() const
{
  return CommandID::Take;
}

void Take::execute(std::vector<std::string>)
{
  //TODO
}
