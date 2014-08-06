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

  virtual void loadStyle(INIFile *inifile);
  virtual void handlePlayerInput(std::string cmd);
  virtual void append(std::string txt, Font efont);
  virtual void appendAnim(std::string text, Font efont, int interval = 10);
  virtual void appendBlank();
  virtual void clear();

private slots:
  void on_c_msg_returnPressed();
};

#endif // GuiConsole_H
