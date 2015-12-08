#ifndef JOBSTATUSTEST
#define JOBSTATUSTEST

#include <QtTest>
#include "../src/jenkinsjob.h"
#include <array>

class jobStatusTest : public QObject
{
    Q_OBJECT

    std::array<JobStatus, 5> status;

public:
    jobStatusTest() {
        status = {JobStatus::UNKNOWN, JobStatus::RUNNING, JobStatus::INSTABLE, JobStatus::SUCCESS, JobStatus::FAILURE};
    }

private slots:
    void testAnd_unknown();
    void testAnd_running();
    void testAnd_instable();
    void testAnd_success();
    void testAnd_failure();

    void testAnd_reflexive();
    void testAnd_symmetric();
};

#endif // JOBSTATUSTEST

