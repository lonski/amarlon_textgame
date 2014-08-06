#ifndef E_COMMAND_H
#define E_COMMAND_H

enum class CommandID
{
/* Nie zapomnij dodac komendy do Command::create_by_enum! */
  Null = 0,
  Go = 1,
  Take = 2,
  Exit = 3,
  GameMenu = 4,
  Debug = 5,
  End = 6
};

#endif // E_COMMAND_H
