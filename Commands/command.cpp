#include <QDebug>
#include "command.h"
#include "take.h"
#include "go.h"
#include "exit.h"
#include "gamemenu.h"
#include "debug.h"
#include "Include/exceptions.h"
#include "Console/console.h"

#include "Include/functions/string_utils.h"

using namespace std;

Command::Command(Console *console)
  : _finished(false)
  , _console(console)
{
}

Console *Command::console() const
{
  return _console;
}

void Command::setConsole(Console *console)
{
  _console = console;
}

void Command::cAppend(string txt, Font efont)
{
  if (_console != nullptr)
  {
    _console->append(txt, efont);
  }
  else
  {
    qDebug() << txt.c_str();
  }
}

void Command::cAppendAnim(string txt, Font efont, int interval)
{
  if (_console != nullptr)
  {
    _console->appendAnim(txt, efont, interval);
  }
  else
  {
    qDebug() << txt.c_str();
  }
}

void Command::cAppendBlank()
{
  if (_console != nullptr)
  {
    _console->appendBlank();
  }
  else
  {
    qDebug() << "";
  }
}

void Command::cClear()
{
  if (_console != nullptr)
  {
    _console->clear();
  }
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

Command *Command::createByEnum(CommandID cmd, Console* console)
{
  Command *r = nullptr;

  switch(cmd)
  {
    case CommandID::Take:
      r = new Take(console);
      break;
    case CommandID::Go:
      r = new Go(console);
      break;
    case CommandID::Exit:
      r = new Exit(console);
      break;
    case CommandID::GameMenu:
      r = new GameMenu(console);
      break;
    case CommandID::Debug:
      r = new Debug(console);
      break;
    default:
      throw error::creation_error("Nieprawidłowy ID komendy! Prawdopodobnie zapomniano dodać id do enum.h.");
      break;
  }

  return r;
}
