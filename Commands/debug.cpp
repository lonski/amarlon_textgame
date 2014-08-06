#include "debug.h"
#include "Gui/game.h"
#include "Creatures/player.h"
#include "Equipment/item.h"
#include "Equipment/weapon.h"
#include "World/location.h"
#include "Equipment/item_container.h"
#include "Creatures/creature.h"

using namespace std;

Debug::Debug()
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
    else if (p == "reload_GuiConsole_skin")
    {
      reloadGuiConsoleSkin();
    }
    else if (p == "clear_GuiConsole")
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
  Command *cmd = _GuiConsole->cmd_exec._active_command;
  int cmd_id = ( cmd == nullptr ? -1 : static_cast<int>(cmd->id()) );

  _GuiConsole->append("DEBUG: " + fun::toStr(cmd_id), Font::Standard);
}

void Debug::reloadGuiConsoleSkin()
{
  _StyleConfig->reload();
  _GuiConsole->load_skin(_StyleConfig);
  _GuiConsole->append("DEBUG: przeładowano konfiguracje stylu z pliku "+Game::styleConfigFilename, Font::Standard);
}

void Debug::saveStyleConfig()
{
  _StyleConfig->save();
  _GuiConsole->append("DEBUG: zapisano styleConfig do pliku "+Game::styleConfigFilename, Font::Standard);
}

void Debug::displayHelp()
{
  _GuiConsole->append("### DEBUG Help ###",Font::Header);
  _GuiConsole->append("active_cmd",Font::Standard);
  _GuiConsole->append("reload_GuiConsole_skin",Font::Standard);
  _GuiConsole->append("save_style_config",Font::Standard);
  _GuiConsole->append("clear_GuiConsole",Font::Standard);
  _GuiConsole->append("create_loc_obj_szafka",Font::Standard);
  _GuiConsole->append("print_creatures",Font::Standard);
  _GuiConsole->append("print_locations",Font::Standard);
}

void Debug::clearGuiConsole()
{
  _GuiConsole->clear();
}

void Debug::createLocObj()
{
  //create object to be inserted into loc
  LocationObjectPtr lobj (LocationObject::forge(ItemPrototype::BlankLocationObject));
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

  _GuiConsole->append("DEBUG: stworzono szafkę i wrzucono do aktualnej lokacji.", Font::Standard);
}

void Debug::printCreatures()
{
  _GuiConsole->append(Creature::Manager.getPrintableContent(), Font::Standard);
}

void Debug::printLocation()
{
  _GuiConsole->append(Location::Manager.getPrintableContent(), Font::Standard);
}
