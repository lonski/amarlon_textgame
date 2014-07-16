#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include "Include/inc.h"
#include "Include/func.h"

class CommandParser
{
private:
  std::string _cmd;
  std::vector<std::string> _params;
public:
  CommandParser(std::string raw_command = "");

  std::string parse(std::string raw_command);
  std::string get_cmd() const
  {
    return _cmd;
  }
  std::vector<std::string> get_params() const
  {
    return _params;
  }
};

#endif // COMMANDPARSER_H
