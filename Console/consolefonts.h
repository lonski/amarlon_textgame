#ifndef ConsoleFonts_H
#define ConsoleFonts_H

#include <QColor>
#include <QFont>
#include "Include/inifile.h"
#include "Include/enums/e_font.h"

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

#endif // ConsoleFonts_H
