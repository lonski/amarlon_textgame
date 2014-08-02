#ifndef COMMAND_H
#define COMMAND_H

#include "Include/inc.h"
#include "Include/gameclock.h"
#include "Include/enums.h"

class Command
{
private:
  bool _finished;
protected:
  std::set<std::string> _cmdNames;
  virtual void setFinished();
  virtual void setNotFinished();
public:
  Command();
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

  static Command* createByEnum(CommandID cmd);
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
  virtual bool isActiveCommand() const
  {
    return true;
  }

  virtual ~ActiveCommand() {}
};

class NonActiveCommand : public Command
{
public:
  virtual bool isActiveCommand() const
  {
    return false;
  }
  virtual ~NonActiveCommand() {}
};

#endif // COMMAND_H
