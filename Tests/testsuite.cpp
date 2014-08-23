#include "testsuite.h"
#include <iostream>
#include <QDebug>

std::vector<QObject*> QTestSuite::m_suites;

QTestSuite::QTestSuite() : QObject()
{
    qDebug() << "c";
    m_suites.push_back(this);
}
