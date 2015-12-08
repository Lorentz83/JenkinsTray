#include <QApplication>

#include <QTest>

#include "jobstatustest.h"
#include "parsetest.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    jobStatusTest jobStatus;
    parseTest parse;

    return QTest::qExec(&jobStatus) & QTest::qExec(&parse) ;
}
