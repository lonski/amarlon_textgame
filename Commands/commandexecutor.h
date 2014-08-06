#ifndef COMMANDEXECUTOR_H
#define COMMANDEXECUTOR_H


#include "commandlogger.h"
#include "commandparser.h"
#include "command.h"

class CommandExecutor
{
private:
  friend class Debug;
  CommandLogger _logger;
  CommandParser _parser;
  Command* _active_command;
  std::map<CommandID, Command*> _commands;
public:
  CommandExecutor();
  ~CommandExecutor();

  bool add_command(Command* cmd);
  bool erase_command(CommandID cmd_id);  
  bool execute(std::string command);
  CommandLogger& log() { return _logger; }
};

#endif // COMMANDEXECUTOR_H
