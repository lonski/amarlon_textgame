#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QLayout>

#include "console.h"

#define _Game Game::inst()

namespace Ui {
  class Game;
}

class Game : public QMainWindow
{
  Q_OBJECT
private:
  explicit Game(QWidget *parent = 0);

  static Game *_instance;
  Console *_console;  

public:  
  ~Game();
  static Game* inst();
  Console* console() { return _console; }
  
private:
  Ui::Game *ui;
};

#endif // GAME_H
