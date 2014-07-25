#ifndef CONSOLEFONTS_H
#define CONSOLEFONTS_H

#include "Include/inc.h"
#include "Include/inifile.h"

#define STRINGIFY_FONT(ENUM) case ENUM: r = #ENUM; break;

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

struct FontConf{
  QColor kolor;
  QFont::Weight weight;
  bool italic;
  FontConf(QColor k = Qt::black, QFont::Weight w = QFont::Normal, bool i = false)
    : kolor(k), weight(w), italic(i)
  {}
};

class ConsoleFonts
{
private:
  std::map<Font, FontConf> _fonts;
public:
  ConsoleFonts();
  FontConf& get(Font font);
  void add(Font font, FontConf conf);
  void load();
  void save();
};

#endif // CONSOLEFONTS_H
