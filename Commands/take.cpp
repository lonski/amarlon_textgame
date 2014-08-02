#include "take.h"
#include "game.h"

Take::Take()
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
