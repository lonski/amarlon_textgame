#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QLayout>

#include "Include/inifile.h"
#include "console.h"

#define _Console Game::console
#define _StyleConfig Game::styleConfig

namespace Ui {
  class Game;
}

class Game : public QMainWindow
{
  Q_OBJECT
public:
  static const std::string styleConfigFilename;  
  static Console *console;
  static INIFile * styleConfig;

public:  
  explicit Game(QWidget *parent = 0);
  ~Game();

private:
  Ui::Game *ui;
};

#endif // GAME_H
