#ifndef TESTCOMMANDS_H
#define TESTCOMMANDS_H

#include "testsuite.h"

#include <QtTest/QtTest>
#include "Commands/commandlogger.h"
#include "Commands/commandparser.h"
#include "Commands/commandexecutor.h"

class TestCommands : public QTestSuite
{
  Q_OBJECT
public:
  TestCommands() {}
private slots:
   void TestCommandLogger();
   void TestCommandParser();
   void TestCommandExecutor();
};

static TestCommands instance;

#endif // TESTCOMMANDS_H
