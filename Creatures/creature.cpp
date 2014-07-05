#include "creature.h"
#include "mob.h"
#include "npc.h"

using namespace std;
using namespace soci;
using namespace fun;

const dbTable Creature::table_name = "creatures";

Creature::Creature(dbRef ref, bool temp)
  : DBObject(ref, temp)
  , _sex(Sex::Null)
  , _mods(this)
  , _total_damage(DamageLevel::Brak)
{  
}

Creature::~Creature()
{
  if ( !isTemporary() && ref() != 0)
  {
    try
    {
      save_to_db();
    }
    catch(std::exception &e)
    {
      qDebug() << "Error saving " << table_name.c_str() << " " << ref() << " : " << e.what();
    }
    catch(...)
    {
      qDebug() << "Error saving " << table_name.c_str() << " "  << ref() << ".";
    }
  }
}

std::unique_ptr<Creature> Creature::create(dbRef ref, bool prototype, bool temp)
{
  Creature* new_crt = nullptr;

  MapRow crt_data = MapQuery("SELECT crt_type, obj_type FROM "+table_name+" WHERE ref="+toStr(ref));
  CreatureType crt_type = CheckFieldCast<CreatureType>( crt_data["CRT_TYPE"] );
  ObjType obj_type = CheckFieldCast<ObjType>( crt_data["OBJ_TYPE"] );

  if (crt_type != CreatureType::Null && (obj_type == ObjType::Instance || prototype) )
  {
    switch(crt_type)
    {
      case CreatureType::MOB: new_crt = new MOB(ref, temp); break;
      case CreatureType::NPC: new_crt = new NPC(ref, temp); break;
      case CreatureType::Player: /*TODO*/ break;
      default : throw error::creation_error("Nieprawidłowy typ itemu."); break;
    }
  }else throw error::creation_error("Brak prawidłowego rekordu w bazie.");

  new_crt->load();

  return unique_ptr<Creature>(new_crt);
}

std::unique_ptr<Creature> Creature::clone()
{
  if (!isTemporary())
  {
    //save
    save_to_db();

    //clone db record
    dbRef new_ref(0);
    _Database << "EXECUTE PROCEDURE CLONE_CRT("<< ref() << ")", into(new_ref);
    _Database.commit();

    //return new item
    return Creature::create(new_ref);
  }

  return unique_ptr<Creature>(nullptr);
}

void Creature::load()
{
  if ( !loaded() && ref() > 0 )
  {
    try
    {
      //DATA
      MapRow crt_data = MapQuery( "SELECT * FROM "+table()+" WHERE ref="+toStr(ref()) );
      if (crt_data.size() > 0)
      {
        //base data
        set_name( CheckField<string>(crt_data["NAME"]) );
        set_descript( CheckField<string>(crt_data["DESCRIPT"]) );
        set_sex( CheckFieldCast<Sex>(crt_data["SEX"]));

        //stats
        _stats.Str2Attributes( CheckField<string>(crt_data["ATTRIBUTES"]) );
        _stats.Str2Skills( CheckField<string>(crt_data["SKILLS"]) );

        //body
        _body.load(CheckField<string>(crt_data["BODY"]));
      }

      //MODS
        //zaladuj z crt_mods
      vector<int> mod_refs(100);
      vector<indicator> inds;

      string query = "SELECT ref FROM crt_mods WHERE otable='" + table() + "' and oref=" + fun::toStr(ref());
      _Database << query, into(mod_refs, inds);

      for (auto m = mod_refs.begin(); m != mod_refs.end(); ++m)
        _mods.add( shared_ptr<CreatureModificator>(new CreatureModificator(*m)) );

        //dodaj z itemów założonych na body_parts
      for (auto im = _body.equipped_items().begin(); im != _body.equipped_items().end(); ++im)
      {
        if (nullptr != *im && !(*im)->mods().get_all().empty())
          _mods.add( (*im)->mods().get_complex_mod() );
      }

      //INVENTORY
      try
      {
        _inventory = Item::Container<>::create(Item::Container<>::byOwner( table(),ref() ));
      }
      catch(error::creation_error)
      {
        _inventory = Item::Container<>::prototypes().clone(ItemContainerPrototype::Inventory);
        _inventory->set_otable(table());
        _inventory->set_oref(ref());
        _inventory->save_to_db();
      }

      calc_total_damage();
      set_loaded();
    }
    catch(soci_error &e)
    {
      MsgError(e.what());
      qDebug() << _Database.get_last_query().c_str();
    }
  }
}

void Creature::save_to_db()
{
  string body_save = _body.toStr();
  save("UPDATE "+table()+" SET body='"+body_save+"' WHERE ref="+fun::toStr(ref()));

  DBObject::save_to_db();
}

void Creature::purge()
{
  _inventory->purge();
  DBObject::purge();
}

void Creature::set_name(string name)
{
  _name = name;
  save("NAME", _name);
}

void Creature::set_descript(string descript)
{
  _descript = descript;
  save("DESCRIPT", _descript);
}

void Creature::set_sex(Sex sex)
{
  _sex = sex;
  save("SEX",static_cast<int>(_sex));
}

void Creature::set_attribute(Attribute atr, int val)
{
  _stats.set_attribute(atr,val);
  save("ATTRIBUTES", _stats.Attributes2Str());
}

void Creature::mod_attribute(Attribute atr, int mod)
{
  _stats.mod_attribute(atr, mod);
  save("ATTRIBUTES", _stats.Attributes2Str());
}

void Creature::set_skill(Skill skill, int val)
{
  _stats.set_skill(skill, val);
  save("SKILLS", _stats.Skills2Str());
}

void Creature::mod_skill(Skill skill, int mod)
{
  _stats.mod_skill(skill, mod);
  save("SKILLS", _stats.Skills2Str());
}

void Creature::take(std::shared_ptr<Item> item, int amount)
{
  _inventory->insert(item, amount);
}

AmountedItem<Item> Creature::drop(dbRef item_ref, int amount)
{
  return _inventory->erase(item_ref, amount);
}

std::vector<AmountedItem<Item> > Creature::inventory()
{
  return _inventory->get_all();
}

void Creature::equip(std::shared_ptr<Item> item)
{
  _body.equip(item);

  if (!item->mods().get_all().empty())
  {
    _mods.add( item->mods().get_complex_mod() );
  }
}

shared_ptr<Item> Creature::unequip(dbRef item_ref)
{
  shared_ptr<Item> r = _body.unequip(item_ref);

  //usun modyfikatory
  if (r != nullptr)
  {
    _mods.remove(r->mods().get_complex_mod()->ref());
  }

  return r;
}

void Creature::calc_total_damage()
{
  //TODO
  //algorytm z systemu rpg
  //na podstawie damage_level z body_parts
}

//==============BODY
void Creature::Body::load(string body_str)
{
  _equipped_items.clear();
  _parts.clear();

  vector<string> body = fun::explode( body_str, ';' );
  for (auto b = body.begin(); b != body.end(); ++b)
    _parts.push_back( shared_ptr<BodyPart>(new BodyPart(*b, _equipped_items) ) );
}

string Creature::Body::toStr()
{
  string str;
  for (auto b = _parts.begin(); b!=_parts.end(); ++b)
  {
    BodyPart *bp = (*b).get();
    str += bp->toStr() + ";";
  }
  return str;
}


void Creature::Body::equip(std::shared_ptr<Item> item)
{
  vector<BodyPartType> req_parts = item->body_parts();
  vector<shared_ptr<BodyPart> > temp_eq_parts;

  //sprawdz czy mamy dostepna ilosc partsów
  for (auto r_part = req_parts.begin(); r_part != req_parts.end(); ++r_part)
  {
    shared_ptr<BodyPart> prefer_temp(nullptr);
    auto prefer_iter = req_parts.end();

    for (auto part = _parts.begin(); part != _parts.end(); ++part)
    {
      shared_ptr<BodyPart> p = *part;      
      //jezeli jest aktualnie szukany part i jest wolny
      if (p->type() == *r_part && p->accept(item->type()) )
      {
        //jeżeli weapon to preferuj RIGHT, jeżeli shield to preferuj LEFT
        if ( (item->type() == ItemType::Weapon && p->side() != BodySide::Right) ||
             (item->type() == ItemType::Shield && p->side() != BodySide::Left) )
        {
          prefer_temp = p;
        }
        else
        //zaloz item i usun typ bodyparta z wymaganych
        {
          prefer_temp.reset();
          p->equip(item);
          temp_eq_parts.push_back(p);
          req_parts.erase(r_part);
          --r_part;
          break;
        }
      }      
    }
    //jeżeli nei znaleziono preferowanej strony ciała dla itema, to załóż na inną dostępną
    if (prefer_temp != nullptr && prefer_iter != req_parts.end())
    {
      prefer_temp->equip(item);
      temp_eq_parts.push_back(prefer_temp);
      req_parts.erase(prefer_iter);
    }
  }
  //zabrakło jakeigos bodyparta -> rollback i exception
  if (!req_parts.empty())
  {
    for (auto r = temp_eq_parts.begin(); r != temp_eq_parts.end(); ++r)
      (*r)->unequip(item->type());

    throw error::equip_no_bodyparts("Brak dostępnych części ciała aby założyć przedmiot "+item->name() + ", ref = " + fun::toStr(item->ref()));
  }

  _equipped_items.push_back(item);
}

std::shared_ptr<Item> Creature::Body::unequip(dbRef item_ref)
{
  ItemType itype = ItemType::Null;
  shared_ptr<Item> result;

  //znajdz typ itema
  for (auto eq = _equipped_items.begin(); eq != _equipped_items.end(); ++eq)
  {
    if ( (*eq)->ref() == item_ref )
    {
      itype = (*eq)->type();
      break;
    }
  }

  //sciagnij ze wszystkich partów
  for (auto part = _parts.begin(); part != _parts.end(); ++part)
  {
    shared_ptr<BodyPart> p = *part;
    if (p->equipped(itype).lock() != nullptr && p->equipped(itype).lock()->ref() == item_ref )
    {
      result = p->unequip(itype);
    }
  }

  if ( result != nullptr )
  {
    //usun z listy equipped
    for (auto eq = _equipped_items.begin(); eq != _equipped_items.end(); ++eq)
    {
      if ( (*eq)->ref() == result->ref() )
      {
        _equipped_items.erase(eq);
        break;
      }
    }
  }

  return result;
}

shared_ptr<BodyPart> Creature::Body::part(BodyPartType type, BodyRegion region, BodySide side)
{
  shared_ptr<BodyPart> bp;

  for (auto b = _parts.begin(); b != _parts.end(); ++b)
  {
    bp = *b;
    if (  (bp->type() == type) &&
          (bp->region() == region || region == BodyRegion::Null) &&
          (bp->side() == side || side == BodySide::Null)
       )
    {
      return bp;
    }
  }

  return bp;
}
