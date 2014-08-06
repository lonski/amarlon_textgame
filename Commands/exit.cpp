#include "exit.h"
#include "Console/console.h"

using namespace std;

Exit::Exit(Console *console)
  : ActiveCommand(console)
  , stage(Stage::Start)
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
  cAppend("Czy jesteś pewien, że chcesz opuścić Amarlon? [t/n]", Font::Message);
  stage = Stage::Asked;
}

void Exit::exit_game()
{
  cAppendAnim("Opuszczasz Amarlon...", Font::Action, 50);

  exit(0);
}

void Exit::cancel_exit()
{
  cAppend("Zatem kontynuujmy.", Font::Action);
  setFinished();
}
