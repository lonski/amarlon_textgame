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

bool CommandExecutor::execute(string command, bool force)
{
  bool r = false;

  if (!command.empty())
  {
    _logger.log(command);
    string cmd_name = _parser.parse(command);

    //jeżeli jest aktywna komenda to ją uruchom
    if (!_active_commands.empty() && !force)
    {
      Command *act_cmd = _active_commands.top();
      act_cmd->execute(command);
      if (act_cmd->is_finished())
      {
        _active_commands.pop();
      }
    }
    //jezeli brak aktywnej komendy, to wyszukaj i odpal pasującą
    else
    {
      for (auto c = _commands.begin(); c != _commands.end(); ++c)
      {
        Command *cmd = c->second;
        if (cmd->accept(cmd_name))
        {
          cmd->execute(_parser.get_params());
          if (cmd->is_active_command())
          {
            _active_commands.push(cmd);
          }
          r = true;
          break;
        }
      }
    }

  }

  return r;
}
