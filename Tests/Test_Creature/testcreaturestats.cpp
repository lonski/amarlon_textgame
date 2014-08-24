#include "testcreaturestats.h"
#include "../Creatures/creaturestats.h"

TestCreatureStats::TestCreatureStats()
{
}

void TestCreatureStats::stats_atr2str()
{
  CreatureStats stats;

  stats.setAttribute(Attribute::CHR, 2);
  stats.setAttribute(Attribute::IMP, 5);
  stats.setAttribute(Attribute::STR, 7);
  stats.setAttribute(Attribute::DEX, 3);

  QCOMPARE(stats.attributes2str().c_str(), "0;7;3;0;0;0;2;5;0;");
}

void TestCreatureStats::stats_str2atrs()
{
  CreatureStats stats;

  stats.str2attributes("0;7;3;9;0;0;2;5;0;");

  QCOMPARE(stats.attribute(Attribute::CHR), 2);
  QCOMPARE(stats.attribute(Attribute::IMP), 5);
  QCOMPARE(stats.attribute(Attribute::STR), 7);
  QCOMPARE(stats.attribute(Attribute::DEX), 3);
  QCOMPARE(stats.attribute(Attribute::END), 9);
  QCOMPARE(stats.attribute(Attribute::REF), 0);
  QCOMPARE(stats.attribute(Attribute::INT), 0);
  QCOMPARE(stats.attribute(Attribute::WLL), 0);
}

void TestCreatureStats::stats_skills2str()
{
  CreatureStats stats;

  stats.setSkill(Skill::Aktorstwo, 43);
  stats.setSkill(Skill::Empatia, 12);
  stats.setSkill(Skill::Mocna_glowa, 67);

  QCOMPARE( stats.skills2str().c_str(), "0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;12;0;0;0;0;0;0;0;0;43;0;0;0;0;0;0;0;0;0;0;0;67;0;0;0;0;0;0;");
}

void TestCreatureStats::stats_str2skills()
{
  CreatureStats stats;

  stats.str2skills("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;12;0;0;0;0;0;0;0;0;43;0;0;0;0;0;0;0;0;0;0;0;67;0;0;0;0;0;1;");

  QCOMPARE(stats.skill(Skill::Aktorstwo), 43);
  QCOMPARE(stats.skill(Skill::Empatia), 12);
  QCOMPARE(stats.skill(Skill::Mocna_glowa), 67);
  QCOMPARE(stats.skill(Skill::Taniec), 1);
  QCOMPARE(stats.skill(Skill::Taktyka), 0);
}

