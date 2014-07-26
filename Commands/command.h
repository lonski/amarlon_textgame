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
  std::set<std::string> _cmd_names;
  virtual void set_finished();
  virtual void set_not_finished();
public:
  Command();
  virtual ~Command() {}

  virtual void tick_game_clock(Minute minutes);

  virtual CommandID id() const = 0;
  virtual bool accept(std::string cmd);
  virtual void execute(std::vector<std::string> params) = 0;  
  virtual void execute(std::string str = "");
  virtual bool is_active_command() const = 0;
  virtual bool is_finished() const;
  virtual void add_name(std::string name);
  virtual void erase_name(std::string name);

  static Command* create_by_enum(CommandID cmd);
};

class ActiveCommand : public Command
{
protected:
  virtual void reset_status() = 0;
  virtual void set_finished()
  {
    reset_status();
    Command::set_finished();
  }

public:
  virtual bool is_active_command() const
  {
    return true;
  }

  virtual ~ActiveCommand() {}
};

class NonActiveCommand : public Command
{
public:
  virtual bool is_active_command() const
  {
    return false;
  }
  virtual ~NonActiveCommand() {}
};

#endif // COMMAND_H
