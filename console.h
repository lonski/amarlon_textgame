#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>
#include <QScrollBar>
#include <QKeyEvent>


#include "Commands/commandexecutor.h"
#include "Include/inifile.h"
#include "consolefonts.h"

#define cDebug(MSG) _Console->append("DEBUG: "+std::string(MSG), FontStandard);

//colors
#define QBrown QColor(110,70,10)

namespace Ui {
  class Console;
}

class Console : public QWidget
{
  Q_OBJECT
public:
  static std::string Divider;

private:
  friend class Debug;
  Ui::Console* ui;
  CommandExecutor cmd_exec;
  ConsoleFonts FontsManager;

  void retrive_command_history(QKeyEvent *event);
  void load_fonts(INIFile *inifile);
  void load_controls_skin(INIFile *inifile);

protected:
  void keyPressEvent(QKeyEvent *event);

public:
  explicit Console(QWidget *parent = 0);  
  ~Console();

  void load_skin(INIFile *inifile);
  void handle_player_input(std::string cmd);
  void append(std::string txt, Font efont);
  void append_anim(std::string text, Font efont, int interval = 10);
  void append_blank();
  void clear();

private slots:
  void on_c_msg_returnPressed();
};

#endif // CONSOLE_H
