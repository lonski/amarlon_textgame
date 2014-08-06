#include "gamemenu.h"
#include "game.h"
#include "exit.h"
#include "go.h"
#include "Creatures/player.h"
#include "World/location.h"

#include "Include/enums/e_refdict.h"

using namespace std;

GameMenu::GameMenu()
  : stage(Stage::Start)
  , subCmd(nullptr)
{
  addName("menu");
}

CommandID GameMenu::id() const
{
  return CommandID::GameMenu;
}

void GameMenu::resetStatus()
{
  stage = Stage::Start;
}

void GameMenu::execute(std::vector<std::string> params)
{
  string input = ( params.empty() ? "" : params[params.size()-1] );

  setNotFinished();

  if (stage == Stage::Start)
  {
    welcomeScreen();
  }
  else if (stage == Stage::Exit)
  {
    subCmd->execute(params);
    if (subCmd->isFinished())
    {
      delete subCmd;
      subCmd = nullptr;
      welcomeScreen();
    }
  }
  else if ( input == "1")
  {
    startNewGame();
  }
  else if ( input == "2")
  {
    if (nullptr == subCmd) subCmd = new Exit;
    stage = Stage::Exit;
    subCmd->execute();
  }

}

void GameMenu::welcomeScreen()
{
  _Console->clear();
  _Console->append("Witaj w Amarlonie!", Font::Header);
  _Console->append("1. Nowa gra", Font::Standard);
  _Console->append("2. Wyjście", Font::Standard);
  _Console->append_blank();

  stage = Stage::Menu;
}

void GameMenu::startNewGame()
{
  _Console->append_anim("Zaczynamy nową grę...", Font::Message, 50);
  _Console->clear();

  setDatabaseForNewGame();
  setPlayerForNewGame();
  displayStartLocation();

  setFinished();
}

void GameMenu::setDatabaseForNewGame()
{  
  //string db_file = "/home/spszenguo/Projects/amarlon/Data/data2.fdb";
  string db_file = "/home/pi/db/data.fdb";
  string db_log_file = "../amarlon/Data/db.log";
  string db_server = "192.168.1.5";

  DB::SetDatabaseInfo(db_file, db_server, db_log_file);
}

void GameMenu::setPlayerForNewGame()
{
  Location* startLocation = new Location( (int)refDict::Location::StartLocation );
  startLocation->load();
  _Player->setLocation(startLocation);
}

void GameMenu::displayStartLocation()
{
  Go go_cmd;
  go_cmd.execute("rozejrzyj");
}

