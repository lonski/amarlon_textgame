#include "exit.h"
#include "Gui/game.h"

using namespace std;

Exit::Exit()
  : stage(Stage::Start)
{
  addName("exit");
}

void Exit::resetStatus()
{
  stage = Stage::Start;
}

CommandID Exit::id() const
{
  return CommandID::Exit;
}

void Exit::execute(std::vector<std::string> params)
{
  string input = ( params.empty() ? "" : params[params.size()-1] );

  if (stage == Stage::Start)
  {
    ask_if_sure();
  }
  else if (input == "t")
  {
    exit_game();
  }
  else
  {
    cancel_exit();
  }
}

void Exit::ask_if_sure()
{
  _GuiConsole->append("Czy jesteś pewien, że chcesz opuścić Amarlon? [t/n]", Font::Message);
  stage = Stage::Asked;
}

void Exit::exit_game()
{
  _GuiConsole->append_anim("Opuszczasz Amarlon...", Font::Action, 50);
  _StyleConfig->save();
  exit(0);
}

void Exit::cancel_exit()
{
  _GuiConsole->append("Zatem kontynuujmy.", Font::Action);
  setFinished();
}
