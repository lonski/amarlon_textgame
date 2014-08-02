#include "debug.h"
#include "game.h"

using namespace std;

Debug::Debug()
{
  addName("debug");
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
    else if (params[1] == "reload_console_skin")
    {
      _StyleConfig->reload();
      _Console->load_skin(_StyleConfig);
      _Console->append("DEBUG: przeładowano konfiguracje stylu z pliku "+Game::styleConfigFilename, Font::Standard);
    }
    else if (params[1] == "clear_console")
    {
      _Console->clear();
    }
    else if (params[1] == "save_style_config")
    {
      _StyleConfig->save();
      _Console->append("DEBUG: zapisano styleConfig do pliku "+Game::styleConfigFilename, Font::Standard);
    }
  }
  else //help
  {
    _Console->append("### DEBUG Help ###",Font::Header);
    _Console->append("active_cmd",Font::Standard);
    _Console->append("reload_console_skin",Font::Standard);
    _Console->append("save_style_config",Font::Standard);
    _Console->append("clear_console",Font::Standard);
  }
}
