#ifndef EXIT_H
#define EXIT_H

#include "command.h"

class Exit : public ActiveCommand
{
protected:
  virtual void reset_status();

private:
  enum class Stage
  {
    Start = 0,
    Asked = 1
  } stage;

  void ask_if_sure();
  void exit_game();
  void cancel_exit();

public:
  Exit();
  virtual ~Exit() {}

  virtual CommandID id() const;
  virtual void execute(std::vector<std::string>);  
};

#endif // EXIT_H
