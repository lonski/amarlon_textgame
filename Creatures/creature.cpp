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
      default : throw creation_error("Nieprawidłowy typ itemu."); break;
    }
  }else throw creation_error("Brak prawidłowego rekordu w bazie.");

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
        vector<string> body = fun::explode( CheckField<string>(crt_data["BODY"]), ';' );
        for (auto b = body.begin(); b != body.end(); ++b)
          _body.push_back( shared_ptr<BodyPart>(new BodyPart(*b)) );
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
      for (auto im = _body.begin(); im != _body.end(); ++im)
      {
        BodyPart *bp = im->get();
        Item *eq = bp->equipped().lock().get();

        if (nullptr != eq && !eq->mods().get_all().empty())
          _mods.add( shared_ptr<CreatureModificator>(&eq->mods().get_complex_mod()) );
      }

      //INVENTORY
      try
      {
        _inventory = Item::Container<>::create(Item::Container<>::byOwner( table(),ref() ));
      }
      catch(creation_error)
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
  string body_save;
  for (auto b = _body.begin(); b!=_body.end(); ++b)
  {
    BodyPart *bp = (*b).get();
    body_save += bp->toStr() + ";";
  }
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

std::vector<std::weak_ptr<BodyPart> > Creature::body_parts()
{
  vector<weak_ptr<BodyPart> > parts;
  for (auto b = _body.begin(); b != _body.end(); ++b)
    parts.push_back(weak_ptr<BodyPart>(*b));

  return parts;
}

void Creature::take(std::shared_ptr<Item> item, int amount)
{
  _inventory->insert(item, amount);
}

void Creature::drop(dbRef item_ref, int amount)
{
  _inventory->erase(item_ref, amount);
}

void Creature::calc_total_damage()
{
  //TODO
  //algorytm z systemu rpg
  //na podstawie damage_level z body_parts
}
