#include "commandexecutor.h"

using namespace std;

CommandExecutor::CommandExecutor()
{
}

CommandExecutor::~CommandExecutor()
{
  for (auto i = _commands.begin(); i != _commands.end(); ++i)
  {
    delete i->second;
  }
}

bool CommandExecutor::add_command(Command *cmd)
{
  bool r = false;

  if (cmd != nullptr)
  {
    Command*& inserted_cmd = (_commands.insert(make_pair<CommandID, Command*>(cmd->id(), nullptr)).first)->second;
    if (inserted_cmd == nullptr)
    {
      inserted_cmd = cmd;
      r = true;
    }
  }

  return r;
}

bool CommandExecutor::erase_command(CommandID cmd_id)
{
  bool r = false;
  auto iter = _commands.find(cmd_id);

  if (iter != _commands.end())
  {
    delete iter->second;
    _commands.erase(iter);
    r = true;
  }

  return r;
}

bool CommandExecutor::execute(string command)
{
  bool r = false;

  if (!command.empty())
  {
    _logger.log(command);
    string cmd_name = _parser.parse(command);

    for (auto c = _commands.begin(); c != _commands.end(); ++c)
    {
      Command *cmd_exec = c->second;
      if (cmd_exec->accept(cmd_name))
      {
        cmd_exec->execute(_parser.get_params());
        r = true;
        break;
      }
    }
  }

  return r;
}
