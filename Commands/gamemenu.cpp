#include "gamemenu.h"
#include "game.h"
#include "exit.h"
#include "go.h"
#include "Creatures/player.h"
#include "World/location.h"

using namespace std;

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

}

void GameMenu::welcome_screen()
{
  _Console->clear();
  _Console->append("Witaj w Amarlonie!", Console::Font::MessageBold);
  _Console->append("1. Nowa gra", Console::Font::Standard);
  _Console->append("2. Wyjście", Console::Font::Standard);
  _Console->append_blank();

  stage = Stage::Menu;
}

void GameMenu::start_new_game()
{
  _Console->append_anim("Zaczynamy nową grę...", Console::Font::Message, 50);
  _Console->clear();

  //set database
  string db_file = "/home/pi/db/data.fdb";
  string db_log_file = "../amarlon/Data/db.log";
  string db_server = "192.168.1.5";//"lonski.pl";

  DB::SetDatabaseInfo(db_file, db_server, db_log_file);

  //set player
  Location* start_location = Location::create((int)refDict::Location::StartLocation);
  start_location->load();
  _Player->set_location(start_location);

  //load location
  Go go_cmd;
  go_cmd.execute("rozejrzyj");

  set_finished();
}
