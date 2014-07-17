#include "testcommands.h"
#include "Commands/go.h"
#include "Commands/take.h"

using namespace std;

void TestCommands::TestCommandLogger()
{
  CommandLogger logger(3);
  string c1 = "jakas ladna komenda";
  string c2 = "podnies sztylet";
  string c3 = "wloz 5 miedziakow do szafki";
  string c4 = "lel";

  QCOMPARE(logger.retrive(0).c_str(), "");
  QCOMPARE(logger.retrive(1).c_str(), "");

  logger.log(c1);
  QCOMPARE(logger.retrive(0).c_str(), c1.c_str());

  logger.log(c2);
  QCOMPARE(logger.retrive(0).c_str(), c2.c_str());
  QCOMPARE(logger.retrive(1).c_str(), c1.c_str());

  logger.log(c3);
  QCOMPARE(logger.retrive(0).c_str(), c3.c_str());
  QCOMPARE(logger.retrive(1).c_str(), c2.c_str());
  QCOMPARE(logger.retrive(2).c_str(), c1.c_str());

  logger.log(c4);
  QCOMPARE(logger.retrive(0).c_str(), c4.c_str());
  QCOMPARE(logger.retrive(1).c_str(), c3.c_str());
  QCOMPARE(logger.retrive(2).c_str(), c2.c_str());
  QCOMPARE(logger.retrive(3).c_str(), "");

}

void TestCommands::TestCommandParser()
{
  CommandParser parser("wez 5 miedziakow");
  QCOMPARE(parser.get_cmd().c_str(), "wez");
  QCOMPARE(parser.get_params().size(), (size_t)2);
  QCOMPARE(parser.get_params().at(0).c_str(), "5");
  QCOMPARE(parser.get_params().at(1).c_str(), "miedziakow");

  parser.parse("lol");
  QCOMPARE(parser.get_cmd().c_str(), "lol");
  QCOMPARE(parser.get_params().size(), (size_t)0);
}

void TestCommands::TestCommandExecutor()
{
  CommandExecutor exec;
  exec.add_command(Command::create(CommandID::Take));
  exec.add_command(Command::create(CommandID::Go));

  QVERIFY(exec.execute("n"));
  QVERIFY(exec.execute("s s s s"));
  QVERIFY(exec.execute("wez"));
  QVERIFY(exec.execute("zabierz"));
  QVERIFY(!exec.execute("dupa"));

  exec.erase_command(CommandID::Go);
  QVERIFY(!exec.execute("n"));
  QVERIFY(exec.execute("podnies tylek lol"));
}
