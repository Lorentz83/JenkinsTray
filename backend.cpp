#include "backend.h"
#include <QNetworkReply>
#include <QTextCodec>
#include <QXmlQuery>
#include <QXmlResultItems>
#include <QBuffer>
#include <QRegExp>
#include <QDateTime>

Backend::Backend(Configuration *configuration, QObject *parent) :
    QObject(parent),
    _configuration(configuration),
    _netManager(parent)
{

    connect(&_netManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(netResponse(QNetworkReply*)));
}

void Backend::refresh() {
    qDebug() << "get "<<_configuration->url;
    _netManager.get(QNetworkRequest(QUrl(_configuration->url)));
}

void Backend::netResponse(QNetworkReply* reply){
    QVector<JenkinsJob> projectsStatus;

    if (reply->error() != QNetworkReply::NoError) {
        emit statusUpdated(projectsStatus, reply->errorString());
        return;
    }

    QByteArray encoded = reply->readAll();
    reply->deleteLater();

    //QString resp = QTextCodec::codecForHtml(encoded,QTextCodec::codecForName("UTF-8"))->toUnicode(encoded);

    QBuffer buffer(&encoded); // This is a QIODevice.
    buffer.open(QIODevice::ReadOnly);

    QXmlQuery queryEntry;
    queryEntry.bindVariable("rss", &buffer);
    queryEntry.setQuery("declare default element namespace \"http://www.w3.org/2005/Atom\"; "
                   "declare variable $rss external; "
                   "for $entry in doc($rss)/feed/entry "
                   "return fn:concat($entry/title/string(), ' ', $entry/link[@type='text/html']/@href)");


    QRegExp rx("^(.+) #([0-9]+) \\((.*)\\) (http.*)$");

    QStringList projects;
    queryEntry.evaluateTo(&projects);

    foreach (QString str, projects) {
        if (rx.indexIn(str) == -1) {
            qDebug() << "error parsing " << str;
        } else {
            QString name = rx.cap(1);
            int buildNumber = rx.cap(2).toInt();
            JobStatus status = parseJobStatus(rx.cap(3));
            QString url = rx.cap(4);
            projectsStatus.append(JenkinsJob(name, buildNumber, status, url));
        }
    }

    emit statusUpdated(projectsStatus, "Last update: " + QDateTime::currentDateTime().toString());
}
