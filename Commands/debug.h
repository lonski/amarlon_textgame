#ifndef DEBUG_H
#define DEBUG_H

#include "command.h"

class Debug : public NonActiveCommand
{
public:
  Debug();
  virtual ~Debug() {}
  virtual CommandID id() const;
  virtual void execute(std::vector<std::string> params);

private:
  void cmdexecActiveCommand();
  void reloadConsoleSkin();
  void saveStyleConfig();
  void displayHelp();
  void clearConsole();
  void createLocObj();
  void printCreatures();
  void printLocation();
};


#endif // DEBUG_H
