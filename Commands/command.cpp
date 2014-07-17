#include "command.h"
#include "take.h"
#include "go.h"

Command::Command()
{
}

void Command::tick_game_clock(Minute minutes)
{
  GameClock::Clock().tick_time(minutes);
}

bool Command::accept(std::string cmd)
{
  return _cmd_names.find(cmd) != _cmd_names.end();
}

void Command::add_name(std::string name)
{
  _cmd_names.insert(name);
}

void Command::erase_name(std::string name)
{
  _cmd_names.erase(name);
}

Command *Command::create(CommandID cmd)
{
  Command *r = nullptr;

  switch(cmd)
  {
    case CommandID::Take:
      r = new Take;
      break;
    case CommandID::Go:
      r = new Go;
      break;
    default:
      r = nullptr;
      break;
  }

  return r;
}
