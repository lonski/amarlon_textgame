#include "console.h"
#include "ui_console.h"

using namespace std;

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

  //ustaw focus
  ui->c_msg->setFocus();

}

Console::~Console()
{  
  delete ui;
}

void Console::load_skin(INIFile *inifile)
{
  load_fonts(inifile);
  load_controls_skin(inifile);
}

void Console::load_fonts(INIFile* inifile)
{
  FontsManager.load(inifile);
}

void Console::load_controls_skin(INIFile *inifile)
{
  //load log style
  string style;

  string bg_color = inifile->getValue("console","bg_color");
  if (!bg_color.empty())
  {
    QColor kolor = fun::Str2Color(bg_color);
    style += " background-color: " + kolor.name().toStdString() + ";";
  }

  ui->c_log->setStyleSheet(style.c_str());
  //~~~
  //load input control style
  bg_color = inifile->getValue("console","input_bg_color");
  if (!bg_color.empty())
  {
    QColor kolor = fun::Str2Color(bg_color);
    style += " background-color: " + kolor.name().toStdString() + ";";
  }
  ui->c_msg->setStyleSheet(style.c_str());

  int input_size = fun::CheckField<int>(inifile->getValue("console","input_font_size"));
  if (input_size)
  {
    QFont font = ui->c_msg->font();
    font.setPointSize(input_size);
    ui->c_msg->setFont(font);
  }
  //~~~
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

void Console::append(std::string txt, Font efont)
{
  FontConf& font = FontsManager.get(efont);

  ui->c_log->setFontItalic(font.italic);
  ui->c_log->setFontPointSize(font.size);
  ui->c_log->setFontWeight(font.weight);
  ui->c_log->setFontFamily(font.family.c_str());

  ui->c_log->setTextColor(font.kolor);
  ui->c_log->setAlignment(Qt::AlignJustify);

  ui->c_log->append(txt.c_str());
  ui->c_log->verticalScrollBar()->setValue(ui->c_log->verticalScrollBar()->maximum());
  ui->c_log->setFontItalic(false);
}

void Console::append_anim(std::string text, Font efont, int interval)
{
  FontConf& font = FontsManager.get(efont);

  QString txt = text.c_str();
  QString fw_b, fw_e;

  switch(font.weight){
    case QFont::Bold: fw_b = "<b>"; fw_e = "</b>"; break;
    default : fw_b = ""; fw_e = ""; break;
  }

  if (font.italic)
    fw_b = "<i>"; fw_e = "</i>";

  QString fsize = fun::toStr(font.size).c_str();
  QString ffamily = font.family.c_str();
  fw_b += "<span style=\"color: " + font.kolor.name() + "; font-size:"+ fsize +"pt; font-family:"+ ffamily +";\">";
  fw_e =+ "</span>";

  QString::iterator it = txt.begin();
  append("", efont);
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
  append("", Font::Standard);
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
