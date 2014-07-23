#include "debug.h"
#include "game.h"

using namespace std;

Debug::Debug()
{
  add_name("debug");
}

CommandID Debug::id() const
{
  return CommandID::Debug;
}

void Debug::execute(std::vector<std::string> params)
{  
  if (params.size() > 1)
  {       
    if (params[1] == "active_cmd")
    {
      Command *cmd = _Console->cmd_exec._active_command;
      int cmd_id = ( cmd == nullptr ? -1 : static_cast<int>(cmd->id()) );

      _Console->append("DEBUG: " + fun::toStr(cmd_id), Console::Font::Standard);
    }
    else if (params[1] == "reload_fonts")
    {
      _Console->load_fonts(Console::fonts_filename);
      _Console->append("DEBUG: przeładowano konfiguracje czcionek z pliku "+Console::fonts_filename, Console::Font::Standard);
    }
    else if (params[1] == "clear")
    {
      _Console->clear();
    }
  }
  else //help
  {
    _Console->append("### DEBUG Help ###",Console::Font::MessageBold);
    _Console->append("active_cmd",Console::Font::Standard);
    _Console->append("reload_fonts",Console::Font::Standard);
    _Console->append("clear",Console::Font::Standard);
  }
}
