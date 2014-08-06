#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>
#include "Include/enums/e_font.h"

class INIFile;

class Console
{
public:
  virtual std::string divider() { static std::string d("-~=====~-"); return d;}

  virtual void loadStyle(INIFile *inifile) = 0;
  virtual void handlePlayerInput(std::string cmd) = 0;
  virtual void append(std::string txt, Font efont) = 0;
  virtual void appendAnim(std::string text, Font efont, int interval = 10) = 0;
  virtual void appendBlank() = 0;
  virtual void clear() = 0;

  virtual ~Console() {}
};



#endif // CONSOLE_H
