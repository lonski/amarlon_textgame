#ifndef CONSOLEFONTS_H
#define CONSOLEFONTS_H

#include <QColor>
#include <QFont>
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
  Header,
  LocExits,
  LocObject,
  LocCreature,

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
    STRINGIFY_FONT(Font::Header)
    STRINGIFY_FONT(Font::LocExits)
    STRINGIFY_FONT(Font::LocObject)
    STRINGIFY_FONT(Font::LocCreature)
    default: break;
  }
  return r;
}

struct FontConf{
  QColor kolor;
  QFont::Weight weight;
  bool italic;
  double size;
  std::string family;
  FontConf(QColor k = Qt::black, QFont::Weight w = QFont::Normal, bool i = false, double fsize = 10.0, std::string ffamily = "verdana")
    : kolor(k), weight(w), italic(i), size(fsize), family(ffamily)
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
  void load(INIFile *inifile);
  void save(INIFile* inifile);
};

#endif // CONSOLEFONTS_H
