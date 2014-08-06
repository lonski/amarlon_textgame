#include "Gui/game.h"
#include "ui_game.h"

const std::string Game::styleConfigFilename = "style_config.ini";
GuiConsole* Game::console(nullptr);
INIFile* Game::styleConfig(nullptr);

Game::Game(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::Game)  
{
  ui->setupUi(this);

  styleConfig = new INIFile(styleConfigFilename);

  console = new GuiConsole(this);
  console->load_style(styleConfig);
  console->show();

  this->setCentralWidget(console);
}

Game::~Game()
{
  delete ui;
  delete console;
  delete styleConfig;
}
