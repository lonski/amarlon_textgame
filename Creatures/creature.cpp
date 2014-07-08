#include "creature.h"
#include "mob.h"
#include "npc.h"

using namespace std;
using namespace soci;
using namespace fun;

const dbTable Creature::table_name = "creatures";
const dbTable Creature::Container::table_name = "crt_containers";

Creature::Creature(dbRef ref, bool temp)
  : DBObject(ref, temp)
  , _sex(Sex::Null)
  , _mods(this)
  , _total_damage(DamageLevel::Brak)
  , _weapon(nullptr)
  , _offhand(nullptr)
  , _shield(nullptr)
{  
}

void Creature::calc_weapons()
{
  _weapon = nullptr;
  _offhand = nullptr;
  _shield = nullptr;

  for ( auto p = _body.parts().begin(); p != _body.parts().end(); ++p )
  {
    BodyPart *bp = p->get();
    Weapon *eq_wpn = dynamic_cast<Weapon*>( bp->equipped(ItemType::Weapon).lock().get() );
    Shield *eq_shd = dynamic_cast<Shield*>( bp->equipped(ItemType::Shield).lock().get() );

    if (eq_wpn != nullptr)
    {
      if (bp->side() == BodySide::Left)
      {
        _offhand = eq_wpn;
      }
      else if (bp->side() == BodySide::Right)
      {
        _weapon = eq_wpn;
      }
    }
    else if (eq_shd != nullptr)
    {
      _shield = eq_shd;
    }
  }
}

Creature::~Creature()
{
  _SAVE_TO_DB_
}

std::unique_ptr<Creature> Creature::create(dbRef ref, bool prototype, bool temp)
{
  Creature* new_crt = nullptr;

  if (ref > 0)
  {
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
    }else throw error::creation_error("Brak prawidłowego rekordu w bazie. Creature ref = "
                                      + fun::toStr(ref) + " crt_type="
                                      + fun::toStr(static_cast<int>(crt_type))
                                      + " obj_type="
                                      + fun::toStr(static_cast<int>(obj_type))
                                      );

    new_crt->load();
  }

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

void Creature::load(MapRow *data_source)
{
  if ( !loaded() && ref() > 0 )
  {
    try
    {
      //DATA
      MapRow crt_data;
      if (data_source != nullptr)
      {
        crt_data = *data_source;
      }
      else
      {
        crt_data = MapQuery( "SELECT * FROM "+table()+" WHERE ref="+toStr(ref()) );
      }

      if (!crt_data.empty())
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
      calc_weapons();
      set_loaded();
      set_not_modified();
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
  stringstream save_query;

  save_query << "UPDATE " << table() << " SET "
             << "  NAME='" << _name << "'"
             << ", DESCRIPT='" << _descript << "'"
             << ", SEX=" << static_cast<int>(_sex)
             << ", ATTRIBUTES='" << _stats.Attributes2Str() << "'"
             << ", SKILLS='" << _stats.Skills2Str() << "'"
             << ", BODY='" << _body.toStr() << "'"
             << " WHERE ref = " << ref();

  save(save_query.str());
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
  set_modified();
}

void Creature::set_descript(string descript)
{
  _descript = descript;  
  set_modified();
}

void Creature::set_sex(Sex sex)
{
  _sex = sex;
  set_modified();
}

void Creature::set_attribute(Attribute atr, int val)
{
  _stats.set_attribute(atr,val);
  set_modified();
}

void Creature::mod_attribute(Attribute atr, int mod)
{
  _stats.mod_attribute(atr, mod);
  set_modified();
}

void Creature::set_skill(Skill skill, int val)
{
  _stats.set_skill(skill, val);
  set_modified();
}

void Creature::mod_skill(Skill skill, int mod)
{
  _stats.mod_skill(skill, mod);
  set_modified();
}

void Creature::take(std::shared_ptr<Item> item, int amount)
{  
  _inventory->insert(item, amount);
  set_modified();
}

AmountedItem<Item> Creature::drop(dbRef item_ref, int amount)
{
  set_modified();
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

  calc_weapons();
  set_modified();
}

shared_ptr<Item> Creature::unequip(dbRef item_ref)
{
  shared_ptr<Item> r = _body.unequip(item_ref);

  //usun modyfikatory
  if (r != nullptr)
  {
    _mods.remove(r->mods().get_complex_mod()->ref());
  }

  calc_weapons();
  set_modified();

  return r;
}

void Creature::calc_total_damage()
{
  //TODO
  //algorytm z systemu rpg
  //na podstawie damage_level z body_parts
}

//==============BODY=============================================================
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


vector<shared_ptr<BodyPart> > Creature::Body::equip(std::shared_ptr<Item> item)
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
  return temp_eq_parts;
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

//===============CREATURE CONTAINER=========================================================
dbRef Creature::Container::byOwner(dbTable otable, dbRef oref)
{
  dbRef ref = 0;
  soci::indicator ind;
  _Database << "SELECT ref FROM crt_containers WHERE otable='"<<otable<<"' and oref="<<fun::toStr(oref), into(ref, ind);  
  if (ind != soci::i_ok) ref = 0;

  return ref;
}

Creature::Container::Container(dbRef ref)
  : DBObject(ref)
{
  load();
}

Creature::Container::Container()
  : DBObject(0)
  , _otable("")
  , _oref(0)
{
  dbRef ref = 0;
  soci::indicator ind;
  _Database << "SELECT new_ref FROM create_crt_container", into(ref, ind);
  _Database.commit();
  if (ind != soci::i_ok) ref = 0;

  DBObject::set_ref(ref);
  DBObject::set_loaded();
  DBObject::set_not_modified();
}

Creature::Container::~Container()
{
  _SAVE_TO_DB_
}

void Creature::Container::load(MapRow *data_source)
{
  if ( !loaded() && ref() > 0 )
  {
    try
    {
      MapRow cont_data;
      if (data_source != nullptr)
      {
        cont_data = *data_source;
      }
      else
      {
        cont_data = fun::MapQuery("SELECT otable, oref, creatures FROM "+table_name+" WHERE ref="+fun::toStr(ref()));
      }

      //==header data
      if (!cont_data.empty())
      {
        set_oref( fun::CheckField<dbRef>(cont_data["OREF"]) );
        set_otable( fun::CheckField<std::string>(cont_data["OTABLE"]) );        
        Str2Creatures( fun::CheckField<std::string>(cont_data["CREATURES"]) );
      }

      set_loaded();
      set_not_modified();
    }
    catch(soci::soci_error &e)
    {
      fun::MsgError(e.what());
      qDebug() << _Database.get_last_query().c_str();
    }
    catch(std::exception &e)
    {
      fun::MsgError(e.what());
    }
  }
}

void Creature::Container::save_to_db()
{
  stringstream save_query;

  save_query << "UPDATE " << table() << " SET"
             << "  CREATURES='" << Creatures2Str() << "'"
             << " ,OTABLE='" << _otable << "'"
             << " ,OREF=" << _oref
             << " WHERE ref=" << ref();

  save(save_query.str());
  DBObject::save_to_db();
}

void Creature::Container::insert(std::shared_ptr<Creature> &crt)
{
  if (_creatures.find(crt->ref()) == _creatures.end())
  {
    _creatures[crt->ref()] = crt;
    set_modified();
  }
  else
  {
    throw error::container_insertion_error("Istota już znajduje się w kontenerze.");
  }
}

std::shared_ptr<Creature> Creature::Container::erase(dbRef crt_ref)
{
  shared_ptr<Creature> r(nullptr);
  auto crt_iter = _creatures.find(crt_ref);

  if (crt_iter != _creatures.end())
  {
    r = crt_iter->second;
    _creatures.erase(crt_iter);
    set_modified();
  }

  return r;
}

std::shared_ptr<Creature> Creature::Container::find(dbRef crt_ref)
{
  shared_ptr<Creature> r(nullptr);
  auto crt_iter = _creatures.find(crt_ref);

  if (crt_iter != _creatures.end())
  {
    r = crt_iter->second;
  }

  return r;
}

std::vector<std::shared_ptr<Creature> > Creature::Container::get_all()
{
  vector<shared_ptr<Creature> > r;

  for (auto c = _creatures.begin(); c != _creatures.end(); ++c)
  {
    r.push_back(c->second);
  }

  return r;
}

void Creature::Container::set_otable(dbTable otable)
{
  _otable = otable;
  set_modified();
}

void Creature::Container::set_oref(dbRef oref)
{
  _oref = oref;
  set_modified();
}


void Creature::Container::Str2Creatures(string crts)
{
  _creatures.clear();
  vector<string> crts_refs = fun::explode(crts,',');
  for (auto c = crts_refs.begin(); c != crts_refs.end(); ++c)
  {
    dbRef ref = fun::fromStr<dbRef>(*c);
    if (ref != 0)
    {
      _creatures[ref] = Creature::create(ref);
    }
  }
}

string Creature::Container::Creatures2Str()
{
  string r("");
  for (auto c = _creatures.begin(); c != _creatures.end(); ++c)
  {
    dbRef ref = c->second->ref();
    if (ref > 0)
    {
      r += fun::toStr(ref) + ",";
    }
  }
  return r;
}
