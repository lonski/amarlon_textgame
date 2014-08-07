#ifndef TESTCOMMANDS_H
#define TESTCOMMANDS_H

#include <QtTest/QtTest>
#include "Commands/commandlogger.h"
#include "Commands/commandparser.h"
#include "Commands/commandexecutor.h"

class TestCommands : public QObject
{
  Q_OBJECT
public:
  TestCommands() {}
private slots:
   void TestCommandLogger();
   void TestCommandParser();
   void TestCommandExecutor();
};

#endif // TESTCOMMANDS_H
