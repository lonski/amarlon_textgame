#include "go.h"
#include "game.h"
#include "Creatures/player.h"
#include "World/location.h"

Go::Go()
{
  add_name("n");
  add_name("s");
  add_name("w");
  add_name("e");
  add_name("rozejrzyj");
}

CommandID Go::id() const
{
  return CommandID::Go;
}

void Go::execute(std::vector<std::string> params)
{
  if (!params.empty())
  {
    execute(params[0]);
  }
}

void Go::execute(std::string direction)
{

  if(!direction.empty())
  {
    if (direction == "rozejrzyj")
    {
      Location* loc = _Player->get_location();

      _Console->append(Console::Divider, Console::Font::Divider);
      _Console->append(loc->name(), Console::Font::LocName);
      _Console->append(loc->descript(), Console::Font::LocDescription);

      loc->loc_walk_within_range(WalkVector(Player::SightRange), &Location::load_no_param);
    }

  }

}
