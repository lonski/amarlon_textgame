#include "go.h"
#include "game.h"
#include "Creatures/player.h"
#include "World/location.h"

using namespace std;

Go::Go()
{
  addName("n");
  addName("s");
  addName("w");
  addName("e");
  addName("nw");
  addName("ne");
  addName("se");
  addName("sw");
  addName("u");
  addName("d");
  addName("rozejrzyj");
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
      rozejrzyjSie();
    }
    else if ( direction == "n")
    {
      movePlayer(Direction::North);
    }
    else if ( direction == "s")
    {
      movePlayer(Direction::South);
    }
    else if ( direction == "e")
    {
      movePlayer(Direction::East);
    }
    else if ( direction == "w")
    {
      movePlayer(Direction::West);
    }
    else if ( direction == "nw")
    {
      movePlayer(Direction::Northwest);
    }
    else if ( direction == "ne")
    {
      movePlayer(Direction::Northeast);
    }
    else if ( direction == "sw")
    {
      movePlayer(Direction::Southwest);
    }
    else if ( direction == "se")
    {
      movePlayer(Direction::Southeast);
    }
    else if ( direction == "u")
    {
      movePlayer(Direction::Up);
    }
    else if ( direction == "d")
    {
      movePlayer(Direction::Down);
    }
  }

}

void Go::rozejrzyjSie()
{
  Location* loc = _Player->getLocation();
  if (loc != nullptr)
  {
    loadNeighbourLocations(loc);
    displayLocationInfo(loc);
  }
}

void Go::movePlayer(Direction direction)
{
  bool moved = false;
  Location* current_loc = _Player->getLocation();

  if(current_loc != nullptr)
  {
    Location *target_loc = current_loc->connection(direction);

    if (target_loc != nullptr)
    {
      _Player->setLocation(target_loc);
      moved = true;
    }
  }

  if(moved)
  {
    movePlayerSuccess();
  }
  else
  {
    movePlayerFail();
  }

}

void Go::movePlayerSuccess()
{
  rozejrzyjSie();
}

void Go::movePlayerFail()
{
  _Console->append("Brak drogi w tą stronę.", Font::Message);
}

void Go::loadNeighbourLocations(Location* loc)
{
  loc->locWalkWithinRange(WalkVector(Player::SightRange), &Location::loadNoParam);
}

void Go::displayLocationInfo(Location* loc)
{
  _Console->append(Console::Divider, Font::Divider);
  _Console->append(loc->name(), Font::LocName);
  displayPossibleExits(loc);
  _Console->append(loc->descript(), Font::LocDescription);
}

void Go::displayPossibleExits(Location *loc)
{
  map<Direction, bool> exits = getPossibleExits(loc);
  string exits_line = "[ ";

  bool appended = false;
  for (auto e = exits.begin(); e != exits.end(); ++e)
  {
    if (e->second)
    {
      if (appended)
        exits_line.append(", ");

      exits_line.append(fun::toLower(fun::Enum2Str(e->first, true)));

      if (!appended)
        appended = true;
    }
  }

  exits_line += " ]";

  _Console->append(exits_line, Font::LocExits);
}

map<Direction, bool> Go::getPossibleExits(Location *loc)
{
  map<Direction, bool> exits;

  for (int d = (int)Direction::Null + 1; d != (int)Direction::End; ++d)
  {
    Direction dir = static_cast<Direction>(d);
    exits[dir] = (loc->connection(dir) != nullptr);
  }

  return exits;
}


