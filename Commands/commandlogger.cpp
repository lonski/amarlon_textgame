#include "commandlogger.h"

CommandLogger::CommandLogger(u16 logsize)
  : log_size(logsize)
{
}

void CommandLogger::log(std::string cmd)
{
  _commands.push_front(cmd);
  if (_commands.size() > log_size)
  {
    _commands.pop_back();
  }
}

std::string CommandLogger::retrive(u16 index)
{
  std::string result;

  if (index < _commands.size())
  {
    result = _commands[index];
  }
  else
  {
    result = "";
  }

  return result;
}
