#include "jobstatustest.h"
#include <iostream>

//{UNKNOWN, RUNNING, INSTABLE, SUCCESS, FAILURE};

void jobStatusTest::testAnd_unknown()
{
    // unknown loses everything
    QCOMPARE( JobStatus::UNKNOWN && JobStatus::RUNNING , JobStatus::RUNNING);
    QCOMPARE( JobStatus::UNKNOWN && JobStatus::FAILURE, JobStatus::FAILURE);
    QCOMPARE( JobStatus::UNKNOWN && JobStatus::INSTABLE, JobStatus::INSTABLE);
    QCOMPARE( JobStatus::UNKNOWN && JobStatus::SUCCESS, JobStatus::SUCCESS);
}

void jobStatusTest::testAnd_running()
{
    // running wins all
    QCOMPARE( JobStatus::RUNNING && JobStatus::UNKNOWN , JobStatus::RUNNING);
    QCOMPARE( JobStatus::RUNNING && JobStatus::SUCCESS, JobStatus::RUNNING);
    QCOMPARE( JobStatus::RUNNING && JobStatus::INSTABLE, JobStatus::RUNNING);
    QCOMPARE( JobStatus::RUNNING && JobStatus::FAILURE, JobStatus::RUNNING);
}

void jobStatusTest::testAnd_instable()
{
    // instable wins success and unknown
    QCOMPARE( JobStatus::INSTABLE && JobStatus::SUCCESS, JobStatus::INSTABLE);
    QCOMPARE( JobStatus::INSTABLE && JobStatus::UNKNOWN, JobStatus::INSTABLE);
}

void jobStatusTest::testAnd_success()
{
    // success wins unknown
    QCOMPARE( JobStatus::SUCCESS && JobStatus::UNKNOWN, JobStatus::SUCCESS);
}

void jobStatusTest::testAnd_failure()
{
    // failure wins all but running
    QCOMPARE( JobStatus::FAILURE && JobStatus::RUNNING , JobStatus::RUNNING);
    QCOMPARE( JobStatus::FAILURE && JobStatus::INSTABLE, JobStatus::FAILURE);
    QCOMPARE( JobStatus::FAILURE && JobStatus::SUCCESS, JobStatus::FAILURE);
    QCOMPARE( JobStatus::FAILURE && JobStatus::UNKNOWN, JobStatus::FAILURE);
}

void jobStatusTest::testAnd_symmetric() {
    for (JobStatus s1 : status_) {
        for (JobStatus s2 : status_) {
            QCOMPARE(s1 && s2, s2 && s1);
        }
    }
}

void jobStatusTest::testAnd_reflexive() {
    for (JobStatus s : status_) {
        QCOMPARE(s && s, s);
    }
}
