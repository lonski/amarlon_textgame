#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QLayout>

#include "Include/inifile.h"
#include "console.h"

#define _Game Game::inst()
#define _Console Game::inst()->console()
#define _StyleConfig Game::inst()->style_config()

namespace Ui {
  class Game;
}

class Game : public QMainWindow
{
  Q_OBJECT
public:
  static const std::string styleConfigFilename;  
private:
  explicit Game(QWidget *parent = 0);

  static Game *_instance;
  Console *_console;  
  INIFile * _styleConfig;


public:  
  ~Game();
  static Game* inst();
  Console* console() { return _console; }
  INIFile* style_config() { return _styleConfig; }


private:
  Ui::Game *ui;
};

#endif // GAME_H
