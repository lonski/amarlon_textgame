#include "testfun.h"

using namespace std;

void TestFun::skill_group()
{
  QCOMPARE(fun::skill_group(Skill::Null), SkillGroup::Null);
  QCOMPARE(fun::skill_group(Skill::End), SkillGroup::Null);

  QCOMPARE(fun::skill_group(Skill::Czytania_Pisanie), SkillGroup::Kaplan);

  QCOMPARE(fun::skill_group(Skill::Inicjatywa), SkillGroup::Wojownik);
  QCOMPARE(fun::skill_group(Skill::Walka_Wrecz), SkillGroup::Wojownik);

  QCOMPARE(fun::skill_group(Skill::Aktorstwo), SkillGroup::Bard);
  QCOMPARE(fun::skill_group(Skill::Piesni), SkillGroup::Bard);

  QCOMPARE(fun::skill_group(Skill::Taniec), SkillGroup::Wspolne);
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


