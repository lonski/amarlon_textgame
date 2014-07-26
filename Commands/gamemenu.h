#ifndef GAMEMENU_H
#define GAMEMENU_H

#include "command.h"

class Exit;

class GameMenu : public ActiveCommand
{
protected:
  virtual void reset_status();

private:
  enum class Stage
  {
    Start = 0,
    Menu = 1,
    Exit = 2
  } stage;

  void welcome_screen();
  void start_new_game();
  Command* sub_cmd;

public:
  GameMenu();
  virtual ~GameMenu() {}

  virtual CommandID id() const;
  virtual void execute(std::vector<std::string>);
};

#endif // GAMEMENU_H
