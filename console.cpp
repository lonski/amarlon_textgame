#include "console.h"
#include "ui_console.h"

Console::Console(QWidget *parent)
  : QWidget(parent)
  , ui(new Ui::Console)
  , font_standard(Qt::black, QFont::Normal, false)
{
  ui->setupUi(this);

  //dodaj wszystkie komendy
  for (int c = (int)CommandID::Null + 1; c != (int)CommandID::End; ++c )
  {
    cmd_exec.add_command( Command::create(static_cast<CommandID>(c)));
  }

}

Console::~Console()
{
  delete ui;
}

void Console::handle_player_cmd(std::string cmd)
{
  cmd_exec.execute(cmd);
  append(ui->c_msg->text().toStdString(), font_standard);
}

void Console::append(std::string txt, const FontConf &font)
{
  ui->c_log->setFontItalic(font.italic);
  ui->c_log->setTextColor(font.kolor);
  ui->c_log->setFontWeight(font.weight);
  ui->c_log->setAlignment(Qt::AlignJustify);

  ui->c_log->append(txt.c_str());
  ui->c_log->verticalScrollBar()->setValue(ui->c_log->verticalScrollBar()->maximum());
  ui->c_log->setFontItalic(false);
}

void Console::on_c_msg_returnPressed()
{
  handle_player_cmd(ui->c_msg->text().toStdString());
  ui->c_msg->clear();
}
