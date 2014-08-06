#include "testfun.h"
#include "Gui/gui_console.h"
#include "Gui/game.h"
#include "Include/functions/string_utils.h"
#include "Include/functions/common_utils.h"

using namespace std;

void TestFun::skill_group()
{
  QCOMPARE(fun::skill_group(Skill::Null), Profession::Null);
  QCOMPARE(fun::skill_group(Skill::End), Profession::Null);

  QCOMPARE(fun::skill_group(Skill::Czytania_Pisanie), Profession::Kaplan);

  QCOMPARE(fun::skill_group(Skill::Inicjatywa), Profession::Wojownik);
  QCOMPARE(fun::skill_group(Skill::Walka_Wrecz), Profession::Wojownik);

  QCOMPARE(fun::skill_group(Skill::Aktorstwo), Profession::Bard);
  QCOMPARE(fun::skill_group(Skill::Piesni), Profession::Bard);

  QCOMPARE(fun::skill_group(Skill::Taniec), Profession::Brak);
}

void TestFun::explode()
{
  vector<string> v;

  //==1
  string str = "asd,xcv,b,";

  v = fun::explode(str,',');

  QCOMPARE(v.size(), (size_t)3 );
  QCOMPARE(v[0].c_str(), "asd");
  QCOMPARE(v[1].c_str(), "xcv");
  QCOMPARE(v[2].c_str(), "b");

  //==2
  v.clear();
  str = "asd,xcv,b";

  QCOMPARE(v.size(), (size_t)0 );
  v = fun::explode(str,',');

  QCOMPARE(v.size(), (size_t)3 );
  QCOMPARE(v[0].c_str(), "asd");
  QCOMPARE(v[1].c_str(), "xcv");
  QCOMPARE(v[2].c_str(), "b");

  //==3
  v.clear();
  str = ",asd,xcv,b,";

  QCOMPARE(v.size(), (size_t)0 );
  v = fun::explode(str,',');

  QCOMPARE(v.size(), (size_t)3 );
  QCOMPARE(v[0].c_str(), "asd");
  QCOMPARE(v[1].c_str(), "xcv");
  QCOMPARE(v[2].c_str(), "b");
}

void TestFun::gameclock()
{
  //initalized state
  QVERIFY(GameClock::Clock().minute() == 0);
  QVERIFY(GameClock::Clock().hour() == 0);
  QVERIFY(GameClock::Clock().day() == 0);
  QVERIFY(GameClock::Clock().month() == 0);
  QVERIFY(GameClock::Clock().year() == 0);

  //increase 4 minutes
  GameClock::Clock().tick_time(4);
  QCOMPARE(GameClock::Clock().minute(), (uint)4 );
  QCOMPARE(GameClock::Clock().hour(),   (uint)0 );
  QCOMPARE(GameClock::Clock().day(),    (uint)0 );
  QCOMPARE(GameClock::Clock().month(),  (uint)0 );
  QCOMPARE(GameClock::Clock().year(),   (uint)0 );

  //increase 1hour 10mins
  GameClock::Clock().tick_time(60 + 10);
  QCOMPARE(GameClock::Clock().minute(), (uint)14 );
  QCOMPARE(GameClock::Clock().hour(),   (uint)1 );
  QCOMPARE(GameClock::Clock().day(),    (uint)0 );
  QCOMPARE(GameClock::Clock().month(),  (uint)0 );
  QCOMPARE(GameClock::Clock().year(),   (uint)0 );

  //increase 3h 2min
  GameClock::Clock().tick_time(3*60 + 2);
  QCOMPARE(GameClock::Clock().minute(), (uint)16 );
  QCOMPARE(GameClock::Clock().hour(),   (uint)4 );
  QCOMPARE(GameClock::Clock().day(),    (uint)0 );
  QCOMPARE(GameClock::Clock().month(),  (uint)0 );
  QCOMPARE(GameClock::Clock().year(),   (uint)0 );

  //increase 2 days
  GameClock::Clock().tick_time(2*24*60);
  QCOMPARE(GameClock::Clock().minute(), (uint)16 );
  QCOMPARE(GameClock::Clock().hour(),   (uint)4 );
  QCOMPARE(GameClock::Clock().day(),    (uint)2 );
  QCOMPARE(GameClock::Clock().month(),  (uint)0 );
  QCOMPARE(GameClock::Clock().year(),   (uint)0 );

  //increase 1month 4dayes 4min
  GameClock::Clock().tick_time(30*24*60 + 4*24*60 + 4);
  QCOMPARE(GameClock::Clock().minute(), (uint)20 );
  QCOMPARE(GameClock::Clock().hour(),   (uint)4 );
  QCOMPARE(GameClock::Clock().day(),    (uint)6 );
  QCOMPARE(GameClock::Clock().month(),  (uint)1 );
  QCOMPARE(GameClock::Clock().year(),   (uint)0 );

  //increase 1year
  GameClock::Clock().tick_time(1*12*30*24*60);
  QCOMPARE(GameClock::Clock().minute(), (uint)20 );
  QCOMPARE(GameClock::Clock().hour(),   (uint)4 );
  QCOMPARE(GameClock::Clock().day(),    (uint)6 );
  QCOMPARE(GameClock::Clock().month(),  (uint)1 );
  QCOMPARE(GameClock::Clock().year(),   (uint)1 );

}

void TestFun::inifile()
{
  INIFile ini("style_conf.ini");

  QVERIFY( ini.setValue("nowa","pik","pok") == false );
  QVERIFY( ini.setValue("nowa","pik2","pok8") == true );
  QCOMPARE(ini.getValue("nowa","pik").c_str(), "pok");
  ini.save();
  ini.load();
  QCOMPARE(ini.getValue("nowa","pik").c_str(), "pok");

  INIFile ini2;
  ini2.open("style_conf.ini");
  ini2.save();

  ini2.eraseSection("nowa");

  QVERIFY( ini2.setValue("nowa","pik","pok") == false );
  QVERIFY( ini2.setValue("nowa","pik2","pok8") == true );
  QCOMPARE(ini2.getValue("nowa","pik").c_str(), "pok");

  ini2.save();
  ini2.load();
  QCOMPARE(ini2.getValue("nowa","pik").c_str(), "pok");
  ini2.eraseSection("nowa");
  ini2.save();
}

