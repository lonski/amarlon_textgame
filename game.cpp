#include "game.h"
#include "ui_game.h"

Game* Game::_instance = nullptr;

Game::Game(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::Game)
{
  ui->setupUi(this);

  _console = new Console(this);
  _console->show();

  this->setCentralWidget(_console);
}

Game::~Game()
{
  delete ui;
  delete _console;
}

Game *Game::inst()
{
  if (nullptr == _instance)
  {
    _instance = new Game;
  }

  return _instance;
}
