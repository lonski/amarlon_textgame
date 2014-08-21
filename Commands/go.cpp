#include "go.h"
#include "Creatures/creature.h"
#include "Creatures/player.h"
#include "World/location.h"
#include "Include/functions/enum2str.h"
#include "Console/console.h"

using namespace std;

Go::Go(Console *console)
  : NonActiveCommand(console)
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
  cAppend("Brak drogi w tą stronę.", Font::Message);
}

void Go::loadNeighbourLocations(Location* loc)
{
  loc->locWalkWithinRange(WalkVector(Player::SightRange), &Location::loadNoParam);
}

void Go::displayLocationInfo(Location* loc)
{
  displayDivider();
  displayLocName(loc);
  displayPossibleExits(loc);
  displayLocDescription(loc);

  displayCreaturesInLoc(loc);
  dispalyLocationObjects(loc);
}

void Go::displayDivider()
{
  cAppend(_console->divider(), Font::Divider);
}

void Go::displayLocName(Location* loc)
{
  cAppend(loc->name(), Font::LocName);
}

void Go::displayLocDescription(Location* loc)
{
  cAppend(loc->descript(), Font::LocDescription);
}

void Go::dispalyLocationObjects(Location* loc)
{
  vector<ItemPtr> objs = loc->getAllObjects();
  for (auto o = objs.begin(); o != objs.end(); ++o)
  {
    ItemPtr obj = *o;
    cAppend(obj->name(), Font::LocObject);
    }
}

void Go::displayCreaturesInLoc(Location *loc)
{
  vector<Creature*> crts = loc->getAllCreatures();

  for(auto c = crts.begin(); c != crts.end(); ++c)
  {
    if (*c != nullptr)
    {/* TODO */
      string name = (*c)->name();
      string loc_dsc = (*c)->locDescript();
      cAppend( (loc_dsc.empty() ? name : loc_dsc), Font::LocCreature);
    }
  }
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

      exits_line.append(fun::toLower(fun::enum2str(e->first, true)));

      if (!appended)
        appended = true;
    }
  }

  exits_line += " ]";

  cAppend(exits_line, Font::LocExits);
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


