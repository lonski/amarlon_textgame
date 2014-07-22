#include "console.h"
#include "ui_console.h"

FontConf Console::FontDivider(Qt::darkGreen, QFont::Bold, true);
FontConf Console::FontLocName(Qt::black, QFont::Bold, true);
FontConf Console::FontLocDescription(Qt::black, QFont::Normal, false);
FontConf Console::FontStandard(Qt::black, QFont::Normal, false);
FontConf Console::FontAction(Qt::black, QFont::Normal, true);
FontConf Console::FontMessage(QBrown, QFont::Normal, false);
FontConf Console::FontMessageBold(QBrown, QFont::Bold, false);

std::string Console::Divider("-~=====~-");

Console::Console(QWidget *parent)
  : QWidget(parent)
  , ui(new Ui::Console)  
{
  ui->setupUi(this);

  //dodaj wszystkie komendy
  for (int c = (int)CommandID::Null + 1; c != (int)CommandID::End; ++c )
  {    
    cmd_exec.add_command( Command::create_by_enum(static_cast<CommandID>(c)));
  }

  ui->c_msg->setFocus();

}

Console::~Console()
{
  delete ui;
}

void Console::handle_player_input(std::string cmd)
{
  cmd_exec.execute(cmd);
}

void Console::retrive_command_history(QKeyEvent *event)
{
  static uint index = -1;

  if(event->key() == Qt::Key_Up)
  {
    if (index+1 < cmd_exec.log().size())
    {
      ++index;
      ui->c_msg->setText( cmd_exec.log().retrive(index).c_str() );
    }
  }

  if(event->key() == Qt::Key_Down)
  {
    if (index > 0)
    {
      --index;
      ui->c_msg->setText( cmd_exec.log().retrive(index).c_str() );
    }
  }

  if (event->key() == Qt::Key_Return)
  {
    index = -1;
  }
}

void Console::keyPressEvent(QKeyEvent *event)
{
  retrive_command_history(event);
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

void Console::append_anim(std::string text, const FontConf &font, int interval)
{
  QString txt = text.c_str();
  QString fw_b, fw_e;

  switch(font.weight){
    case QFont::Bold: fw_b = "<b>"; fw_e = "</b>"; break;
    default : fw_b = ""; fw_e = ""; break;
  }

  fw_b += "<span style=\"color: " + font.kolor.name() + ";\">";
  fw_e =+ "</span>";

  QString::iterator it = txt.begin();
  append("", font);
  while (it != txt.end()){
    ui->c_log->moveCursor (QTextCursor::End);
    QString c ( *it == ' ' ? "&nbsp;" : QString(*it) );

    ui->c_log->insertHtml(fw_b+c+fw_e);
    fun::delay(interval);
    it++;
  }

}

void Console::append_blank()
{
  append("", Console::FontStandard);
}

void Console::clear()
{
  ui->c_log->clear();
}

void Console::on_c_msg_returnPressed()
{
  handle_player_input(ui->c_msg->text().toStdString());  
  ui->c_msg->clear();
}
