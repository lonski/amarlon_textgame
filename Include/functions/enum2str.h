#ifndef ENUM2STR_H
#define ENUM2STR_H

#include <string>

#include "Include/enums/e_itemtype.h"
#include "Include/enums/e_direction.h"
#include "Include/enums/e_body.h"
#include "Include/enums/e_attribute.h"
#include "Include/enums/e_skill.h"

namespace fun
{

std::string enum2str(BodyPartType bp);
std::string enum2str(BodyRegion bp);
std::string enum2str(BodySide bp);
std::string enum2str(ItemType bp);
std::string enum2str(Attribute e);
std::string enum2str(Skill e);
std::string enum2str(Direction e, bool shortMode);

}
#endif // ENUM2STR_H
