#include "gamemenu.h"
#include "game.h"
#include "exit.h"

GameMenu::GameMenu()
  : stage(Stage::Start)
  , exit(nullptr)
{
  add_name("menu");
}

CommandID GameMenu::id() const
{
  return CommandID::GameMenu;
}

void GameMenu::reset_status()
{
  stage = Stage::Start;
}

void GameMenu::execute(std::vector<std::string>)
{
  execute("");
}

void GameMenu::execute(std::string input)
{
  set_not_finished();

  if (stage == Stage::Start)
  {
    welcome_screen();
  }
  else if ( input == "1")
  {
    start_new_game();
  }
  else if ( input == "2")
  {    
    if (nullptr == exit) exit = new Exit;
    stage = Stage::Exit;
    exit->execute("");
  }
  else if (stage == Stage::Exit)
  {
    exit->execute(input);
    if (exit->is_finished())
    {
      delete exit;
      exit = nullptr;
      welcome_screen();
    }
  }
}

void GameMenu::welcome_screen()
{
  _Console->clear();
  _Console->append("Witaj w Amarlonie!", Console::font_message_bold);
  _Console->append("1. Nowa gra", Console::font_standard);
  _Console->append("2. Wyjście", Console::font_standard);
  _Console->append_blank();

  stage = Stage::Menu;
}

void GameMenu::start_new_game()
{
  _Console->append("Zaczynamy nową grę.", Console::font_message);
  set_finished();
}
