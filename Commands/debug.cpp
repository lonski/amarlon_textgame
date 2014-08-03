#include "debug.h"
#include "game.h"
#include "Creatures/player.h"
#include "Equipment/item.h"
#include "Equipment/weapon.h"
#include "World/location.h"
#include "Equipment/item_container.h"

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
    else if (p == "reload_console_skin")
    {
      reloadConsoleSkin();
    }
    else if (p == "clear_console")
    {
      clearConsole();
    }
    else if (p == "save_style_config")
    {
      saveStyleConfig();
    }
    else if (p == "create_loc_obj_szafka")
    {
      createLocObj();
    }
  }
  else
  {
    displayHelp();
  }
}

void Debug::cmdexecActiveCommand()
{
  Command *cmd = _Console->cmd_exec._active_command;
  int cmd_id = ( cmd == nullptr ? -1 : static_cast<int>(cmd->id()) );

  _Console->append("DEBUG: " + fun::toStr(cmd_id), Font::Standard);
}

void Debug::reloadConsoleSkin()
{
  _StyleConfig->reload();
  _Console->load_skin(_StyleConfig);
  _Console->append("DEBUG: przeładowano konfiguracje stylu z pliku "+Game::styleConfigFilename, Font::Standard);
}

void Debug::saveStyleConfig()
{
  _StyleConfig->save();
  _Console->append("DEBUG: zapisano styleConfig do pliku "+Game::styleConfigFilename, Font::Standard);
}

void Debug::displayHelp()
{
  _Console->append("### DEBUG Help ###",Font::Header);
  _Console->append("active_cmd",Font::Standard);
  _Console->append("reload_console_skin",Font::Standard);
  _Console->append("save_style_config",Font::Standard);
  _Console->append("clear_console",Font::Standard);
  _Console->append("create_loc_obj_szafka",Font::Standard);
}

void Debug::clearConsole()
{
  _Console->clear();
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

  _Console->append("DEBUG: stworzono szafkę i wrzucono do aktualnej lokacji.", Font::Standard);
}
