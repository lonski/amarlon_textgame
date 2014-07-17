#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>
#include <QScrollBar>

#include "Include/enums.h"
#include "Commands/commandexecutor.h"

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
  static const FontConf font_standard;

private:
  Ui::Console *ui;
  CommandExecutor cmd_exec;

public:
  explicit Console(QWidget *parent = 0);  
  ~Console();

  void handle_player_cmd(std::string cmd);
  void append(std::string txt, const FontConf& font);
  void clear();

private slots:
  void on_c_msg_returnPressed();
};

#endif // CONSOLE_H
