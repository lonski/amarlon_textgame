#include "consolefonts.h"
#include "game.h"

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

void ConsoleFonts::load()
{
  for (int f = 0; f != (int)Font::End; ++f)
  {
    Font font = static_cast<Font>(f);
    string section = Font2Str(font);
    FontConf fc;

    fc.italic = fun::CheckField<bool>( _StyleConfig->getValue(section,"italic") );
    fc.weight = fun::CheckFieldCast<QFont::Weight>( _StyleConfig->getValue(section,"weight") );

    string rgb_color = _StyleConfig->getValue(section,"color_rgb");
    vector<string> colors = fun::explode(rgb_color, ';');
    if (colors.size() == 3)
    {
      int r = fun::fromStr<int>(colors[0]);
      int g = fun::fromStr<int>(colors[1]);
      int b = fun::fromStr<int>(colors[2]);
      fc.kolor = QColor(r,g,b);
    }

    _fonts[font] = fc;
  }
}

void ConsoleFonts::save()
{
  for (int f = 0; f != (int)Font::End; ++f)
  {
    Font font = static_cast<Font>(f);
    string section = Font2Str(font);
    FontConf fc = _fonts[font];

    _StyleConfig->setValue(section,"italic", fun::toStr(fc.italic));
    _StyleConfig->setValue(section,"weight", fun::toStr(fc.weight));
    stringstream ss;
    ss << fc.kolor.red() << ";"
       << fc.kolor.green() << ";"
       << fc.kolor.blue();
    _StyleConfig->setValue(section,"color_rgb", ss.str());
  }

  _StyleConfig->save();
}
