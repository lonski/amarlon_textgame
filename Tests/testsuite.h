#ifndef TESTSUITE_H
#define TESTSUITE_H

#include <QObject>
#include <vector>

class QTestSuite : public QObject
{
    Q_OBJECT
public:
    static std::vector<QObject*> m_suites;

public:
    explicit QTestSuite();

};

#endif // TESTSUITE_H
