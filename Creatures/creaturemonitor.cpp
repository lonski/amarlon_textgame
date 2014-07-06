#include "creaturemonitor.h"

using namespace std;

CreatureMonitor::CreatureMonitor(Creature *crt)
{
  set_creature(crt);
}

CreatureMonitor::CreatureMonitor()
{
}

void CreatureMonitor::set_creature(Creature *crt)
{
  _crt = crt;
  calc();
}

void CreatureMonitor::calc()
{
}

string CreatureMonitor::print()
{
  string str;
  str += "\n=====  =====  ===== CREATURE MONITOR =====  =====  =====";
  str += "\nName: " + _crt->name() + ", Ref: " + fun::toStr(_crt->ref());
  str += "\n======  =====  =====  =====  =====  =====  =====  ======";
  str += "\n"+print_bodyparts();
  str += "\n"+print_eq();
  str += "\n"+print_inventory();
  str += "\n"+print_mods();
  str += "\n======  =====  =====  =====  =====  =====  =====  ======";
  return str+"\n";
}

string CreatureMonitor::print_bodyparts()
{
  stringstream ss;
  ss << "\n===== BODY PARTS =====";

  Creature::Body::BodyParts& parts = _crt->body()._parts;
  for (auto p = parts.begin(); p!=parts.end(); ++p)
  {
    BodyPart *bp = p->get();
    string str;

    //dane podstawowe
    str += "\n### " + fun::Enum2Str(bp->type())
                 + ", strona: "
                 + fun::Enum2Str(bp->side())
                 + ", region: " + fun::Enum2Str(bp->region())
                 + " ###";

    //armor
    str += "\n\tArmor: obuchowe="
         + fun::toStr( bp->armor().bashing )
         + ", sieczne="
         + fun::toStr( bp->armor().slashing )
         + ", klute="
         + fun::toStr( bp->armor().piercing );

    //obrażenia
    str += "\n\tPoziom obrazen: " + fun::toStr( (int)bp->damage() );

    //equipped
    for (int i = (int)ItemType::Null+1; i != (int)ItemType::End; ++i)
    {
      weak_ptr<Item> itm = bp->equipped(static_cast<ItemType>(i));
      if (itm.lock() != nullptr)
      {
        str += "\n\tZalozony przedmiot ["+fun::Enum2Str(static_cast<ItemType>(i))+"]: " + itm.lock()->name() + ", ref = " + fun::toStr(itm.lock()->ref());
      }
    }

    ss << str;
  }

  return ss.str();
}

string CreatureMonitor::print_inventory()
{
  stringstream ss;
  ss << "\n===== INVENTORY =====";
  for (auto i = _crt->inventory().begin(); i != _crt->inventory().end(); ++i)
  {
    AmountedItem<Item> itm = *i;
    if (itm.item != nullptr)
    {
      ss << "\n\t"+itm.item->name()+ " ["+fun::toStr(itm.amount)+"], ref = " + fun::toStr(itm.item->ref());
    }
  }
  return ss.str();
}

string CreatureMonitor::print_mods()
{
  vector<weak_ptr<CreatureModificator> > mods =  _crt->_mods.get_all();
  stringstream ss;
  ss << "\n===== MODIFICATORS =====";
  for (auto m = mods.begin(); m != mods.end(); ++m)
  {
    CreatureModificator *mod = m->lock().get();
    if (mod != nullptr)
    {
      ss << "\n### Name: " << mod->name();

      bool psnt = false;
      ss << "\n\t";
      for (int i = (int)Attribute::Null+1; i != (int)Attribute::End; ++i)
      {
        Attribute atr = static_cast<Attribute>(i);
        int val = mod->creature_stats().attribute(atr);
        if (val != 0)
        {
          ss << "[ " << fun::Enum2Str(atr) << (val > 0 ? " +" : " " ) << val << " ]";
          psnt = true;
        }
      }

      if (psnt) ss << "\n\t";
      psnt = false;
      for (int i = (int)Skill::Null+1; i != (int)Skill::End; ++i)
      {
        Skill skl = static_cast<Skill>(i);
        int val = mod->creature_stats().skill(skl);
        if (val != 0)
        {
          ss << "[ " << fun::Enum2Str(skl) << (val > 0 ? "+" : "" ) << val << " ]";
          psnt = true;
        }
      }

      if (psnt) ss << "\n\t";
      ss << "Ref = " << mod->ref();
      ss << "\n\toRef = " << mod->oref();
      ss << "\n\toTable = " << mod->otable();
    }
  }
  return ss.str();
}

string CreatureMonitor::print_eq()
{
  stringstream ss;

  if (_crt->weapon() != nullptr || _crt->offhand() != nullptr || _crt->shield() != nullptr)
  {
    ss << "\n===== EQUIPPED WEAPONS =====";
    if (_crt->weapon() != nullptr)
    {
      ss << "\n### Weapon: " << _crt->weapon()->name() + ", ref = " + fun::toStr(_crt->weapon()->ref());
    }
    if (_crt->offhand() != nullptr)
    {
      ss << "\n### Offhand: " << _crt->offhand()->name() + ", ref = " + fun::toStr(_crt->offhand()->ref());
    }
    if (_crt->shield() != nullptr)
    {
      ss << "\n### Shield: " << _crt->shield()->name() + ", ref = " + fun::toStr(_crt->shield()->ref());
    }
  }

  return ss.str();
}
