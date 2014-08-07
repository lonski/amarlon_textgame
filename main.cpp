#include <QApplication>
#include <QDebug>
#include "Gui/game.h"

int main(int argc, char *argv[])
{
  QApplication amarlon(argc, argv);

  //START GAME
  Game game;
  _GuiConsole->clear();
  _GuiConsole->handlePlayerInput("menu");
  game.show();

  return amarlon.exec();

}
