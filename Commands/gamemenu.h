#ifndef GAMEMENU_H
#define GAMEMENU_H

#include "command.h"

class Exit;

class GameMenu : public ActiveCommand
{
public:
  GameMenu(Console *console);
  virtual ~GameMenu() {}

  virtual CommandID id() const;
  virtual void execute(std::vector<std::string>);

  void displayStartLocation();
protected:
  virtual void resetStatus();

private:
  enum class Stage
  {
    Start = 0,
    Menu = 1,
    Exit = 2
  } stage;

  Command* subCmd;

  void welcomeScreen();
  void startNewGame();
  void setDatabaseForNewGame();
  void setPlayerForNewGame();

};

#endif // GAMEMENU_H
