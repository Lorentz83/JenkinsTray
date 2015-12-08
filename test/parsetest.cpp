#include "parsetest.h"
#include "../src/backend.h"


void parseTest::testParseEmptyProjectList()
{
    QFile rss(":/res/empty.rss");
    rss.open(QIODevice::ReadOnly);

    JenkinsStatus status = parseJenkinsRss(rss);

    QCOMPARE(status.isValid(), true);
    QCOMPARE(status.jobs().empty(), true);
    QCOMPARE(status.errorMessage(), QStringLiteral(""));

}

void parseTest::testParseJunk() {
    QFile rss(":/res/junk.rss");
    rss.open(QIODevice::ReadOnly);

    JenkinsStatus status = parseJenkinsRss(rss);

    QCOMPARE(status.isValid(), false);
    QCOMPARE(status.jobs().empty(), true);
    QCOMPARE(status.errorMessage(), QStringLiteral("Not a jenkins reply"));
}

void parseTest::parseSingleExecution() {
//<?xml version="1.0" encoding="UTF-8"?><feed xmlns="http://www.w3.org/2005/Atom"><title>All all builds</title><link type="text/html" href="http://localhost/jenkins/" rel="alternate"/><updated>2015-12-08T18:51:46Z</updated><author><name>Jenkins Server</name></author><id>urn:uuid:903deee0-7bfa-11db-9fe1-0800200c9a66</id><entry><title>proj 1 #1 (broken since this build)</title><link type="text/html" href="http://localhost/jenkins/job/proj%201/1/" rel="alternate"/><id>tag:hudson.dev.java.net,2015:proj 1:2015-12-08_13-51-46</id><published>2015-12-08T18:51:46Z</published><updated>2015-12-08T18:51:46Z</updated></entry></feed>
}

void parseTest::parseMultipleExecutions() {
    QFile rss(":/res/oneproject_multiplebuilds.rss");
    rss.open(QIODevice::ReadOnly);
    JenkinsStatus status = parseJenkinsRss(rss);

    QCOMPARE(status.isValid(), true);
    QCOMPARE(status.jobs().size(), 1);

    const JenkinsJob &job = status.jobs().at(0);
    QCOMPARE(job.buildNumber(), 7);
    QCOMPARE(job.name(), QStringLiteral("proj 1"));
    QCOMPARE(job.status(), JobStatus::INSTABLE);
    QCOMPARE(job.url(), QStringLiteral("http://localhost/jenkins/job/proj%201/7/"));

}
