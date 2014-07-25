#include "game.h"
#include "ui_game.h"

Game* Game::_instance = nullptr;
const std::string Game::styleConfigFilename = "style_config.ini";

Game::Game(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::Game)  
{
  ui->setupUi(this);

  _styleConfig = new INIFile(styleConfigFilename);

  _console = new Console(this);
  _console->show();

  this->setCentralWidget(_console);
}

Game::~Game()
{
  delete ui;
  delete _console;
  delete _styleConfig;
}

Game *Game::inst()
{
  if (nullptr == _instance)
  {
    _instance = new Game;
  }

  return _instance;
}
