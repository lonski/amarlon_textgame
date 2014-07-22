#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>
#include <QScrollBar>
#include <QKeyEvent>

#include "Include/enums.h"
#include "Commands/commandexecutor.h"

#define cDebug(MSG) _Console->append("DEBUG: "+std::string(MSG), Console::FontStandard);

//colors
#define QBrown QColor(110,70,10)

namespace Ui {
  class Console;
}

struct FontConf{
  QColor kolor;
  QFont::Weight weight;
  bool italic;
  FontConf(QColor k, QFont::Weight w, bool i)
    : kolor(k), weight(w), italic(i)
  {}
};

class Console : public QWidget
{
  Q_OBJECT
public:
  static FontConf FontDivider;
  static FontConf FontLocName;
  static FontConf FontLocDescription;
  static FontConf FontStandard;
  static FontConf FontAction;
  static FontConf FontMessage;
  static FontConf FontMessageBold;

  static std::string Divider;

private:
  friend class Debug;
  Ui::Console* ui;
  CommandExecutor cmd_exec;

  void retrive_command_history(QKeyEvent *event);

protected:
  void keyPressEvent(QKeyEvent *event);

public:
  explicit Console(QWidget *parent = 0);  
  ~Console();

  void handle_player_input(std::string cmd);
  void append(std::string txt, const FontConf& font);
  void append_anim(std::string text, const FontConf& font, int interval = 10);
  void append_blank();
  void clear();

private slots:
  void on_c_msg_returnPressed();
};

#endif // CONSOLE_H
