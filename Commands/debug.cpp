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
  if (!params.empty())
  {
    if (params[0] == "active_cmd")
    {
      Command *cmd = _Console->cmd_exec._active_command;
      int cmd_id = ( cmd == nullptr ? -1 : static_cast<int>(cmd->id()) );

      _Console->append("DEBUG: " + fun::toStr(cmd_id), Console::font_standard);
    }
  }
  else //help
  {
    _Console->append("### DEBUG Help ###",Console::font_message_bold);
    _Console->append("active_cmd",Console::font_standard);
  }
}
