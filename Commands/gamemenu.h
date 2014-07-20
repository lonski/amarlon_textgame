#ifndef GAMEMENU_H
#define GAMEMENU_H

#include "command.h"

class GameMenu : public ActiveCommand
{
protected:
  virtual void reset_status();

private:
  enum class Stage
  {
    Start = 0,
    Menu = 1
  } stage;

  void welcome_screen();
  void start_new_game();

public:
  GameMenu();
  virtual ~GameMenu() {}

  virtual CommandID id() const;
  virtual void execute(std::vector<std::string>);
  virtual void execute(std::string);
};

#endif // GAMEMENU_H
