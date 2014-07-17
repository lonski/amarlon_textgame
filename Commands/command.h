#ifndef COMMAND_H
#define COMMAND_H

#include "Include/inc.h"
#include "Include/gameclock.h"
#include "Include/enums.h"

class Command
{
protected:
  std::set<std::string> _cmd_names;
public:
  Command();
  virtual ~Command() {}

  virtual void tick_game_clock(Minute minutes);

  virtual CommandID id() const = 0;
  virtual bool accept(std::string cmd);
  virtual void execute(std::vector<std::string> params) = 0;
  virtual void add_name(std::string name);
  virtual void erase_name(std::string name);

  static Command* create(CommandID cmd);
};

#endif // COMMAND_H
