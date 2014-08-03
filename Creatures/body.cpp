#include "body.h"

using namespace std;

//==============BODY=============================================================
void Body::load(string body_str)
{
  _equipped_items.clear();
  _parts.clear();

  vector<string> body = fun::explode( body_str, ';' );
  for (auto b = body.begin(); b != body.end(); ++b)
    _parts.push_back( shared_ptr<BodyPart>(new BodyPart(*b, _equipped_items) ) );
}

string Body::toStr()
{
  string str;
  for (auto b = _parts.begin(); b!=_parts.end(); ++b)
  {
    BodyPart *bp = (*b).get();
    str += bp->toStr() + ";";
  }
  return str;
}


vector<shared_ptr<BodyPart> > Body::equip(ItemPtr item)
{
  vector<BodyPartType> req_parts = item->bodyParts();
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

ItemPtr Body::unequip(dbRef item_ref)
{
  ItemType itype = ItemType::Null;
  ItemPtr result;

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

shared_ptr<BodyPart> Body::part(BodyPartType type, BodyRegion region, BodySide side)
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
