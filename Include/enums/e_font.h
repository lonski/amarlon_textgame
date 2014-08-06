#ifndef E_FONT_H
#define E_FONT_H

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

#endif // E_FONT_H
