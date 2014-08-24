#ifndef TESTCREATURESTATS_H
#define TESTCREATURESTATS_H

#include "testsuite.h"
#include <QtTest/QtTest>

class TestCreatureStats : public QTestSuite
{
  Q_OBJECT
public:
  TestCreatureStats();
private slots:
  void stats_atr2str();
  void stats_str2atrs();
  void stats_skills2str();
  void stats_str2skills();
};

#endif // TESTCREATURESTATS_H
