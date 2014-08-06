#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>
#include "Include/gameclock.h"

#include "Include/enums/e_command.h"
#include "Include/enums/e_font.h"

class Console;

class Command
{
private:
  bool _finished;
protected:
  std::set<std::string> _cmdNames;
  Console* _console;

  virtual void setFinished();
  virtual void setNotFinished();

public:
  Command(Console *console);
  virtual ~Command() {}

  virtual void tickGameClock(Minute minutes);

  virtual CommandID id() const = 0;
  virtual bool accept(std::string cmd);
  virtual void execute(std::vector<std::string> params) = 0;  
  virtual void execute(std::string str = "");
  virtual bool isActiveCommand() const = 0;
  virtual bool isFinished() const;
  virtual void addName(std::string name);
  virtual void eraseName(std::string name);

  static Command* createByEnum(CommandID cmd, Console *console);

  Console *console() const;
  void setConsole(Console *console);

  virtual void cAppend(std::string txt, Font efont);
  virtual void cAppendAnim(std::string txt, Font efont, int interval = 10);
  virtual void cAppendBlank();
  virtual void cClear();

};

class ActiveCommand : public Command
{
protected:
  virtual void resetStatus() = 0;
  virtual void setFinished()
  {
    resetStatus();
    Command::setFinished();
  }

public:
  ActiveCommand(Console *console)
    : Command(console)
  {
  }
  virtual bool isActiveCommand() const
  {
    return true;
  }

  virtual ~ActiveCommand() {}
};

class NonActiveCommand : public Command
{
public:
  NonActiveCommand(Console *console )
    : Command(console)
  {
  }
  virtual bool isActiveCommand() const
  {
    return false;
  }
  virtual ~NonActiveCommand() {}
};

#endif // COMMAND_H
