#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>
#include "Include/enums/e_font.h"

class INIFile;

class Console
{
public:
  virtual void load_style(INIFile *inifile) = 0;
  virtual void handle_player_input(std::string cmd) = 0;
  virtual void append(std::string txt, Font efont) = 0;
  virtual void append_anim(std::string text, Font efont, int interval = 10) = 0;
  virtual void append_blank() = 0;
  virtual void clear() = 0;

  virtual ~Console() {}
};

#endif // CONSOLE_H
