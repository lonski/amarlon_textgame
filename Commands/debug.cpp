#include "debug.h"
#include "Creatures/player.h"
#include "Equipment/item.h"
#include "World/location.h"
#include "Equipment/item_container.h"
#include "Creatures/creature.h"
#include "Commands/commandexecutor.h"
#include "Console/console.h"

using namespace std;

Debug::Debug(Console *console)
  : NonActiveCommand(console)
{
  addName("debug");
}

CommandID Debug::id() const
{
  return CommandID::Debug;
}

void Debug::execute(std::vector<std::string> params)
{  
  if (params.size() > 1)
  {       
    string p = params[1];
    if (p == "active_cmd")
    {
      cmdexecActiveCommand();
    }
    else if (p == "reload_console_skin")
    {
      reloadGuiConsoleSkin();
    }
    else if (p == "clear_console")
    {
      clearGuiConsole();
    }
    else if (p == "save_style_config")
    {
      saveStyleConfig();
    }
    else if (p == "create_loc_obj_szafka")
    {
      createLocObj();
    }
    else if (p == "print_creatures")
    {
      printCreatures();
    }
    else if (p == "print_locations")
    {
      printLocation();
    }
  }
  else
  {
    displayHelp();
  }
}

void Debug::cmdexecActiveCommand()
{
/*
  Command *cmd = _console->cmd_exec->_active_command;
  int cmd_id = ( cmd == nullptr ? -1 : static_cast<int>(cmd->id()) );

  cAppend("DEBUG: " + fun::toStr(cmd_id), Font::Standard);
*/
}

void Debug::reloadGuiConsoleSkin()
{
/*
  _StyleConfig->reload();
  _console->load_style(_StyleConfig);
  cAppend("DEBUG: przeładowano konfiguracje stylu z pliku "+Game::styleConfigFilename, Font::Standard);
*/
}

void Debug::saveStyleConfig()
{
/*
  _StyleConfig->save();
  cAppend("DEBUG: zapisano styleConfig do pliku "+Game::styleConfigFilename, Font::Standard);
*/
}

void Debug::displayHelp()
{
  cAppend("### DEBUG Help ###",Font::Header);
  cAppend("DISABLED active_cmd",Font::Standard);
  cAppend("DISABLED reload_console_skin",Font::Standard);
  cAppend("DISABLED save_style_config",Font::Standard);
  cAppend("clear_console",Font::Standard);
  cAppend("create_loc_obj_szafka",Font::Standard);
  cAppend("print_creatures",Font::Standard);
  cAppend("print_locations",Font::Standard);
}

void Debug::clearGuiConsole()
{
  cClear();
}

void Debug::createLocObj()
{
  //create object to be inserted into loc
  ItemPtr lobj (Item::forge(ItemPrototype::BlankLocationObject));
  lobj->setName("Szafka");
  lobj->setDescript("Niewielka drewniana, prostokątna szafka. Drzwiczki nadgryzł już ząb czasu, a klamka została urwana.");

  //forge some items
  ItemPtr wpn(Item::forge(ItemPrototype::MieczSzeroki));
  ItemPtr arm(Item::forge(ItemPrototype::HelmSkorzany));

  //and add them to obj
  lobj->inventory()->insert(wpn);
  lobj->inventory()->insert(arm);

  //add obj to current location
  Location *loc = _Player->getLocation();
  loc->insertObject(lobj);

  cAppend("DEBUG: stworzono szafkę i wrzucono do aktualnej lokacji.", Font::Standard);
}

void Debug::printCreatures()
{
  cAppend(Creature::Manager.getPrintableContent(), Font::Standard);
}

void Debug::printLocation()
{
  cAppend(Location::Manager.getPrintableContent(), Font::Standard);
}
