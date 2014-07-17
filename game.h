#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QLayout>

#include "console.h"

namespace Ui {
  class Game;
}

class Game : public QMainWindow
{
  Q_OBJECT
private:
  Console *_console;
  
public:
  explicit Game(QWidget *parent = 0);
  ~Game();
  
private:
  Ui::Game *ui;
};

#endif // GAME_H
