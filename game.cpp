#include "game.h"
#include "ui_game.h"

Game::Game(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Game)
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
