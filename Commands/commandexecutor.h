#ifndef COMMANDEXECUTOR_H
#define COMMANDEXECUTOR_H

#include <stack>

#include "Include/inc.h"
#include "commandlogger.h"
#include "commandparser.h"
#include "command.h"

class CommandExecutor
{
private:
  CommandLogger _logger;
  CommandParser _parser;
  std::stack<Command*> _active_commands;
  std::map<CommandID, Command*> _commands;
public:
  CommandExecutor();
  ~CommandExecutor();

  bool add_command(Command* cmd);
  bool erase_command(CommandID cmd_id);  
  bool execute(std::string command, bool force = false);
};

#endif // COMMANDEXECUTOR_H
