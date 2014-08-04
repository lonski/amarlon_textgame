#include "creature.h"
#include "mob.h"
#include "npc.h"
#include "Equipment/item_container.h"
#include "creaturecontainer.h"
#include "World/location.h"

using namespace std;
using namespace soci;
using namespace fun;

const dbTable Creature::tableName = "creatures";
const dbTable Creature::Container::tableName = "crt_containers";
CreatureManager Creature::Manager;

Creature::Creature(dbRef ref, bool temp)
  : DBObject(ref, temp)
  , _sex(Sex::Null)
  , _mods(this)
  , _total_damage(DamageLevel::Brak)
  , _weapon(nullptr)
  , _offhand(nullptr)
  , _shield(nullptr)
  , _currentLoc(nullptr)
  , _prevLoc(nullptr)
{  
}

void Creature::calcWeapons()
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
  _saveToDB_
      Manager.remove(ref());
}

Item::Inventory &Creature::inventoryContainer()
{
  if (_inventory == nullptr)
    {
      _inventory.reset(new Item::Container);
      _inventory->setOTable(table());
      _inventory->setORef(ref());
      _inventory->saveToDB();
    }

  return _inventory;
}

Creature *Creature::create(dbRef ref, bool prototype, bool temp)
{
  Creature* new_crt = nullptr;

  if (ref > 0)
    {
      MapRow crt_data = MapQuery("SELECT crt_type, obj_type FROM "+tableName+" WHERE ref="+toStr(ref));
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

  Manager.add(new_crt);

  return new_crt;
}

Creature* Creature::clone()
{
  if (!isTemporary())
    {
      //save
      saveToDB();

      //clone db record
      dbRef new_ref(0);
      _Database << "EXECUTE PROCEDURE CLONE_CRT("<< ref() << ")", into(new_ref);
      _Database.commit();

      //return new item
      return Creature::create(new_ref);
    }

  return nullptr;
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
            setName( CheckField<string>(crt_data["NAME"]) );
            setDescript( CheckField<string>(crt_data["DESCRIPT"]) );
            setLocDescript( CheckField<string>(crt_data["LOC_DESCRIPT"]) );
            setSex( CheckFieldCast<Sex>(crt_data["SEX"]));

            //stats
            _stats.str2attributes( CheckField<string>(crt_data["ATTRIBUTES"]) );
            _stats.str2skills( CheckField<string>(crt_data["SKILLS"]) );

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

        //dodaj z itemów założonych na bodyParts
        for (auto im = _body.equipped_items().begin(); im != _body.equipped_items().end(); ++im)
          {
            if (nullptr != *im && !(*im)->mods().getAll().empty())
              _mods.add( (*im)->mods().get_complex_mod() );
          }

        //INVENTORY
        dbRef inv_ref = Item::Container::byOwner( table(),ref() );
        if (inv_ref != 0)
          {
            _inventory.reset( new Item::Container(inv_ref));
          }
        else
          {
            _inventory.reset();
          }

        calcTotalDamage();
        calcWeapons();
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

void Creature::saveToDB()
{
  stringstream save_query;

  save_query << "UPDATE " << table() << " SET "
             << "  NAME='" << _name << "'"
             << ", DESCRIPT='" << _descript << "'"
             << ", LOC_DESCRIPT='" << _locDescript << "'"
             << ", SEX=" << static_cast<int>(_sex)
             << ", ATTRIBUTES='" << _stats.attributes2str() << "'"
             << ", SKILLS='" << _stats.skills2str() << "'"
             << ", BODY='" << _body.toStr() << "'"
             << ", LOCATION=" << (getLocation() == nullptr ? 0 : getLocation()->ref())
             << " WHERE ref = " << ref();

  save(save_query.str());
  DBObject::saveToDB();
}

void Creature::purge()
{
  if (inventoryContainer() != nullptr) inventoryContainer()->purge();
  DBObject::purge();
}

string Creature::name() const
{
  return _name;
}

string Creature::locDescript() const
{
  return _locDescript;
}

string Creature::descript() const
{
  return _descript;
}

Sex Creature::sex() const
{
  return _sex;
}

DamageLevel Creature::totalDamage() const
{
  return _total_damage;
}

int Creature::attribute(Attribute atr) const
{
  return _stats.attribute(atr) + _mods.get_complex_mod()->creature_stats().attribute(atr);
}

int Creature::skill(Skill skill) const
{
  return _stats.skill(skill) + _mods.get_complex_mod()->creature_stats().skill(skill);
}

CreatureStats &Creature::stats()
{
  return _stats;
}

Location *Creature::getLocation() const
{
  return _currentLoc;
}

Location *Creature::getPrevLoc() const
{
  return _prevLoc;
}

CreatureModificatorManager &Creature::mods()
{
  return _mods;
}

Body &Creature::body()
{
  set_modified();
  return _body;
}

void Creature::setName(string name)
{
  _name = name;
  set_modified();
}

void Creature::setDescript(string descript)
{
  _descript = descript;
  set_modified();
}

void Creature::setLocDescript(string locDescript)
{
  _locDescript = locDescript;
  set_modified();
}

void Creature::setSex(Sex sex)
{
  _sex = sex;
  set_modified();
}

void Creature::setAttribute(Attribute atr, int val)
{
  _stats.setAttribute(atr,val);
  set_modified();
}

void Creature::modifyAttribute(Attribute atr, int mod)
{
  _stats.modifyAttribute(atr, mod);
  set_modified();
}

void Creature::setSkill(Skill skill, int val)
{
  _stats.setSkill(skill, val);
  set_modified();
}

void Creature::modifySkill(Skill skill, int mod)
{
  _stats.modifySkill(skill, mod);
  set_modified();
}

void Creature::take(ItemPtr item, int amount)
{  
  inventoryContainer()->insert(item, amount);
  set_modified();
}

AmountedItem Creature::drop(dbRef item_ref, int amount)
{
  set_modified();
  return inventoryContainer()->erase(item_ref, amount);
}

std::vector<AmountedItem > Creature::inventory()
{
  return inventoryContainer()->getAll();
}

void Creature::equip(ItemPtr item)
{
  _body.equip(item);

  if (!item->mods().getAll().empty())
  {
    _mods.add( item->mods().get_complex_mod() );
  }

  calcWeapons();
  set_modified();
}

ItemPtr Creature::unequip(dbRef item_ref)
{
  ItemPtr r = _body.unequip(item_ref);

  //usun modyfikatory
  if (r != nullptr)
    {
      _mods.remove(r->mods().get_complex_mod()->ref());
    }

  calcWeapons();
  set_modified();

  return r;
}

Weapon *Creature::weapon()
{
  return _weapon;
}

Weapon *Creature::offhand()
{
  return _offhand;
}

Shield *Creature::shield()
{
  return _shield;
}

void Creature::setLocation(Location *loc)
{
  _prevLoc = _currentLoc;
  _currentLoc = loc;
}

void Creature::calcTotalDamage()
{
  //TODO
  //algorytm z systemu rpg
  //na podstawie damage_level z bodyParts
}
