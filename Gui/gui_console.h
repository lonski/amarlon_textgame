#ifndef GuiConsole_H
#define GuiConsole_H

#include <QWidget>
#include <QScrollBar>
#include <QKeyEvent>

#include "Console/console.h"

#define cDebug(MSG) _GuiConsole->append("DEBUG: "+std::string(MSG), FontStandard);

//colors
#define QBrown QColor(110,70,10)

class CommandExecutor;
class ConsoleFonts;
class INIFile;

namespace Ui {
  class GuiConsole;
}

class GuiConsole : public QWidget
                 , public Console
{
  Q_OBJECT
public:
  static std::string Divider;

private:
  friend class Debug;
  Ui::GuiConsole* ui;
  CommandExecutor* cmd_exec;
  ConsoleFonts* FontsManager;

  void retrive_command_history(QKeyEvent *event);
  void load_fonts(INIFile *inifile);
  void load_controls_skin(INIFile *inifile);

protected:
  void keyPressEvent(QKeyEvent *event);

public:
  explicit GuiConsole(QWidget *parent = 0);  
  virtual ~GuiConsole();

  virtual void load_style(INIFile *inifile);
  virtual void handle_player_input(std::string cmd);
  virtual void append(std::string txt, Font efont);
  virtual void append_anim(std::string text, Font efont, int interval = 10);
  virtual void append_blank();
  virtual void clear();

private slots:
  void on_c_msg_returnPressed();
};

#endif // GuiConsole_H
