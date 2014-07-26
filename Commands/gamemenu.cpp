#include "gamemenu.h"
#include "game.h"
#include "exit.h"
#include "go.h"
#include "Creatures/player.h"
#include "World/location.h"

using namespace std;

GameMenu::GameMenu()
  : stage(Stage::Start)
  , sub_cmd(nullptr)
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

void GameMenu::execute(std::vector<std::string> params)
{
  string input = ( params.empty() ? "" : params[params.size()-1] );

  set_not_finished();

  if (stage == Stage::Start)
  {
    welcome_screen();
  }
  else if (stage == Stage::Exit)
  {
    sub_cmd->execute(params);
    if (sub_cmd->is_finished())
    {
      delete sub_cmd;
      sub_cmd = nullptr;
      welcome_screen();
    }
  }
  else if ( input == "1")
  {
    start_new_game();
  }
  else if ( input == "2")
  {
    if (nullptr == sub_cmd) sub_cmd = new Exit;
    stage = Stage::Exit;
    sub_cmd->execute();
  }

}

void GameMenu::welcome_screen()
{
  _Console->clear();
  _Console->append("Witaj w Amarlonie!", Font::Header);
  _Console->append("1. Nowa gra", Font::Standard);
  _Console->append("2. Wyjście", Font::Standard);
  _Console->append_blank();

  stage = Stage::Menu;
}

void GameMenu::start_new_game()
{
  _Console->append_anim("Zaczynamy nową grę...", Font::Message, 50);
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
