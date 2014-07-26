#include "game.h"
#include "ui_game.h"

const std::string Game::styleConfigFilename = "style_config.ini";
Console* Game::console(nullptr);
INIFile* Game::styleConfig(nullptr);

Game::Game(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::Game)  
{
  ui->setupUi(this);

  styleConfig = new INIFile(styleConfigFilename);

  console = new Console(this);
  console->load_skin(styleConfig);
  console->show();

  this->setCentralWidget(console);
}

Game::~Game()
{
  delete ui;
  delete console;
  delete styleConfig;
}
