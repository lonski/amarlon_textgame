#include "gamemenu.h"
#include "exit.h"
#include "go.h"
#include "Creatures/player.h"
#include "World/location.h"
#include "Console/console.h"

#include "Include/enums/e_refdict.h"

using namespace std;

GameMenu::GameMenu(Console *console)
  : ActiveCommand(console)
  , stage(Stage::Start)
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
    if (nullptr == subCmd) subCmd = new Exit(_console);
    stage = Stage::Exit;
    subCmd->execute();
  }

}

void GameMenu::welcomeScreen()
{
  cClear();
  cAppend("Witaj w Amarlonie!", Font::Header);
  cAppend("1. Nowa gra", Font::Standard);
  cAppend("2. Wyjście", Font::Standard);
  cAppendBlank();

  stage = Stage::Menu;
}

void GameMenu::startNewGame()
{
  cAppendAnim("Zaczynamy nową grę...", Font::Message, 50);
  cClear();

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
  Go* go_cmd = new Go(console());
  go_cmd->execute("rozejrzyj");
  delete go_cmd;
}

