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

      _Console->append("DEBUG: " + fun::toStr(cmd_id), Font::Standard);
    }
    else if (params[1] == "reload_fonts")
    {
      _StyleConfig->reload();
      _Console->load_fonts();
      _Console->append("DEBUG: przeładowano konfiguracje czcionek z pliku "+Game::styleConfigFilename, Font::Standard);
    }
    else if (params[1] == "clear")
    {
      _Console->clear();
    }
  }
  else //help
  {
    _Console->append("### DEBUG Help ###",Font::MessageBold);
    _Console->append("active_cmd",Font::Standard);
    _Console->append("reload_fonts",Font::Standard);
    _Console->append("clear",Font::Standard);
  }
}
