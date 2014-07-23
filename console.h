#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>
#include <QScrollBar>
#include <QKeyEvent>

#include "Include/enums.h"
#include "Commands/commandexecutor.h"
#include "Include/inifile.h"

#define cDebug(MSG) _Console->append("DEBUG: "+std::string(MSG), Console::FontStandard);
#define STRINGIFY_FONT(ENUM) case ENUM: r = #ENUM; break;

//colors
#define QBrown QColor(110,70,10)

namespace Ui {
  class Console;
}

struct FontConf{  
  QColor kolor;
  QFont::Weight weight;
  bool italic;
  FontConf(QColor k = Qt::black, QFont::Weight w = QFont::Normal, bool i = false)
    : kolor(k), weight(w), italic(i)
  {}
};

class Console : public QWidget
{
  Q_OBJECT
public:
  static std::string fonts_filename;
  static std::string Divider;

  enum class Font
  {
    Standard = 0,
    Divider,
    LocName,
    LocDescription,
    Action,
    Message,
    MessageBold,

    End
  };
  static inline std::string Font2Str(Font font)
  {
    std::string r;
    switch(font)
    {
      STRINGIFY_FONT(Font::Standard)      
      STRINGIFY_FONT(Font::Divider)
      STRINGIFY_FONT(Font::LocName)
      STRINGIFY_FONT(Font::LocDescription)
      STRINGIFY_FONT(Font::Action)
      STRINGIFY_FONT(Font::Message)
      STRINGIFY_FONT(Font::MessageBold)
      default: break;
    }
    return r;
  }

private:
  class FontManager
  {
  private:
    INIFile _fonts_ini;
    std::map<Font, FontConf> _fonts;
  public:
    FontConf& get(Font font);
    void add(Font font, FontConf conf);
    void load(std::string file);
    void save(std::string file);
  } FontsManager;

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

  void load_fonts(std::string file);
  void save_fonts(std::string file);
  void handle_player_input(std::string cmd);
  void append(std::string txt, Console::Font efont);
  void append_anim(std::string text, Console::Font efont, int interval = 10);
  void append_blank();
  void clear();

private slots:
  void on_c_msg_returnPressed();
};

#endif // CONSOLE_H
