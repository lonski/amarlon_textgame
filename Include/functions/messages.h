#ifndef MESSAGES_H
#define MESSAGES_H

#include <QDebug>

namespace fun
{

static inline void MsgError(std::string e)
{
  //QMessageBox::critical(NULL, "Error",e.c_str());
  qDebug() << "ERROR: " << e.c_str();
}

}

#endif // MESSAGES_H
