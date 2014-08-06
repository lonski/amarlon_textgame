#include "consolefonts.h"
#include "game.h"
#include "Include/functions/string_utils.h"

using namespace std;

ConsoleFonts::ConsoleFonts()
{
}

FontConf& ConsoleFonts::get(Font font)
{
  return _fonts[font];
}

void ConsoleFonts::add(Font font, FontConf conf)
{
  _fonts[font] = conf;
}

void ConsoleFonts::load(INIFile* inifile)
{
  for (int f = 0; f != (int)Font::End; ++f)
  {
    Font font = static_cast<Font>(f);
    string section = Font2Str(font);
    FontConf fc;

    string ffamily = fun::CheckValue<string>( inifile->getValue(section,"family") );
    if( !ffamily.empty()) fc.family = ffamily;

    double fsize = fun::CheckValue<double>( inifile->getValue(section,"size") );
    if (fsize) fc.size = fsize;

    fc.italic = fun::CheckValue<bool>( inifile->getValue(section,"italic") );

    bool bold = fun::CheckValueCast<bool>( inifile->getValue(section,"bold") );
    fc.weight = ( bold ? QFont::Weight::Bold : QFont::Weight::Normal);

    string rgb_color = inifile->getValue(section,"color_rgb");
    fc.kolor = fun::Str2Color(rgb_color);

    _fonts[font] = fc;
  }
}

void ConsoleFonts::save(INIFile *inifile)
{
  for (int f = 0; f != (int)Font::End; ++f)
  {
    Font font = static_cast<Font>(f);
    string section = Font2Str(font);
    FontConf fc = _fonts[font];

    inifile->setValue(section,"family", fc.family);
    inifile->setValue(section,"size", fun::toStr(fc.size));
    inifile->setValue(section,"italic", fun::toStr(fc.italic));
    inifile->setValue(section,"bold", fun::toStr( static_cast<int>(fc.weight == QFont::Bold) ));
    stringstream ss;
    ss << fc.kolor.red() << ";"
       << fc.kolor.green() << ";"
       << fc.kolor.blue();
    inifile->setValue(section,"color_rgb", ss.str());
  }

  inifile->save();
}
