#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <string>
#include <QTime>
#include <QDebug>
#include <QCoreApplication>

#include "Include/enums/e_profession.h"
#include "Include/enums/e_skill.h"

namespace fun{

Profession skill_group(Skill skill);

//==========
static inline void delay(int amt)
{
  QTime dieTime= QTime::currentTime().addMSecs(amt);

  while( QTime::currentTime() < dieTime )
  {
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
  }
}


}

#endif // COMMON_UTILS_H
