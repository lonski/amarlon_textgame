#include "exit.h"
#include "game.h"

Exit::Exit()
  : stage(Stage::Start)
{
  add_name("exit");
}

void Exit::reset_status()
{
  stage = Stage::Start;
}

CommandID Exit::id() const
{
  return CommandID::Exit;
}

void Exit::execute(std::vector<std::string>)
{
  execute("");
}

void Exit::execute(std::string input)
{
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
  _Console->append("Czy jesteś pewien, że chcesz opuścić Amarlon? [t/n]", Console::font_message);
  stage = Stage::Asked;
}

void Exit::exit_game()
{
  _Console->append("Opuszczasz Amarlon.", Console::font_action);
  fun::delay(1000);
  exit(0);
}

void Exit::cancel_exit()
{
  _Console->append("Zatem kontynuujmy.", Console::font_action);
  set_finished();
}
