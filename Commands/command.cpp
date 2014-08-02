#include "command.h"
#include "take.h"
#include "go.h"
#include "exit.h"
#include "gamemenu.h"
#include "debug.h"
#include "Include/exceptions.h"
#include "Include/func.h"

using namespace std;

Command::Command()
  : _finished(false)
{
}

void Command::setFinished()
{
  _finished = true;
}

void Command::setNotFinished()
{
  _finished = false;
}

void Command::tickGameClock(Minute minutes)
{
  GameClock::Clock().tick_time(minutes);
}

bool Command::accept(std::string cmd)
{
  return _cmdNames.find(cmd) != _cmdNames.end();
}

void Command::execute(string str)
{
  execute(fun::explode(str,' '));
}

bool Command::isFinished() const
{
  return _finished;
}

void Command::addName(std::string name)
{
  _cmdNames.insert(name);
}

void Command::eraseName(std::string name)
{
  _cmdNames.erase(name);
}

Command *Command::createByEnum(CommandID cmd)
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
    case CommandID::Debug:
      r = new Debug;
      break;
    default:
      throw error::creation_error("Nieprawidłowy ID komendy! Prawdopodobnie zapomniano dodać id do enum.h.");
      break;
  }

  return r;
}
