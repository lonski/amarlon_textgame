#include "command.h"
#include "take.h"
#include "go.h"
#include "exit.h"
#include "gamemenu.h"
#include "Include/exceptions.h"

Command::Command()
  : _finished(false)
{
}

void Command::set_finished()
{
  _finished = true;
}

void Command::tick_game_clock(Minute minutes)
{
  GameClock::Clock().tick_time(minutes);
}

bool Command::accept(std::string cmd)
{
  return _cmd_names.find(cmd) != _cmd_names.end();
}

void Command::execute(std::string)
{
  throw error::execution_error("Próba wywołania komendy bez wymaganych parametrów!");
}

bool Command::is_finished() const
{
  return _finished;
}

void Command::add_name(std::string name)
{
  _cmd_names.insert(name);
}

void Command::erase_name(std::string name)
{
  _cmd_names.erase(name);
}

Command *Command::create_by_enum(CommandID cmd)
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
    case CommandID::Exit:
      r = new Exit;
      break;
    case CommandID::GameMenu:
      r = new GameMenu;
      break;
    default:
      throw error::creation_error("Nieprawidłowy ID komendy! Prawdopodobnie zapomniano dodać id do enum.h.");
      break;
  }

  return r;
}
