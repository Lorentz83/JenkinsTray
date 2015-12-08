#ifndef PARSETEST_H
#define PARSETEST_H

#include <QtTest>

class parseTest : public QObject
{
    Q_OBJECT

private slots:

    void testParseEmptyProjectList();
    void testParseJunk();
    void parseSingleExecution();
    void parseMultipleExecutions();
};

#endif // PARSETEST_H
