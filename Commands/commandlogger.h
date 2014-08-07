#ifndef COMMANDLOGGER_H
#define COMMANDLOGGER_H

#include <deque>
#include <string>
#include "Include/typedefs/def_basetypes.h"

class CommandLogger
{
  const u16 log_size;
  std::deque<std::string> _commands;
public:
  CommandLogger(u16 logsize = 1000);

  void log(std::string cmd);
  std::string retrive(u16 index);
  size_t size() const { return _commands.size(); }
};

#endif // COMMANDLOGGER_H
