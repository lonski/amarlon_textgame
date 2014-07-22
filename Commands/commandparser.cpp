#include "commandparser.h"

CommandParser::CommandParser(std::string raw_command)
{
  parse(raw_command);
}

std::string CommandParser::parse(std::string raw_command)
{
  _cmd = "";
  _params.clear();

  _params = fun::explode(raw_command, ' ');
  if (_params.size() > 0)
  {
    _cmd = _params[0];
    //_params.erase(_params.begin());
  }

  return _cmd;
}
