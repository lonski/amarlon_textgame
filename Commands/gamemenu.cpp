#include "gamemenu.h"
#include "game.h"

GameMenu::GameMenu()
  : stage(Stage::Start)
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
    _Console->handle_player_input("exit", true);
  }
}

void GameMenu::welcome_screen()
{
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
