#ifndef COMMANDLOGGER_H
#define COMMANDLOGGER_H

#include "Include/inc.h"

class CommandLogger
{
  const uint log_size;
  std::deque<std::string> _commands;
public:
  CommandLogger(uint logsize = 1000);

  void log(std::string cmd);
  std::string retrive(uint index);
  size_t size() const { return _commands.size(); }
};

#endif // COMMANDLOGGER_H
