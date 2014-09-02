#include "testsuite.h"
#include <iostream>
#include <QDebug>

std::vector<QObject*> QTestSuite::m_suites;

QTestSuite::QTestSuite() : QObject()
{
    m_suites.push_back(this);
}
