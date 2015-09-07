#include "backend.h"
#include <QNetworkReply>
#include <QTextCodec>
#include <QXmlQuery>
#include <QBuffer>
#include <QRegExp>

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
        qDebug() << reply->errorString();
        emit statusUpdated(projectsStatus);
        return;
    }

    QByteArray encoded = reply->readAll();
    reply->deleteLater();

    //QString resp = QTextCodec::codecForHtml(encoded,QTextCodec::codecForName("UTF-8"))->toUnicode(encoded);

    QBuffer buffer(&encoded); // This is a QIODevice.
    buffer.open(QIODevice::ReadOnly);

    QXmlQuery query;


    query.bindVariable("rss", &buffer);
    query.setQuery("declare default element namespace \"http://www.w3.org/2005/Atom\"; "
                   "declare variable $rss external; "
                   "doc($rss)/feed/entry/title/string()");

    QStringList projects;
    query.evaluateTo(&projects);


    QRegExp rx("^(.+) #([0-9]+) \\((.*)\\)$");
    foreach (QString str, projects) {
        if (rx.indexIn(str) == -1) {
            qDebug() << "error parsing " << str;

        } else {
            //qDebug() << rx.cap(1) << rx.cap(2) << rx.cap(3);
            JobStatus status = JobStatus::SUCCESS;
            QString name = rx.cap(1);
            int buildNumber = rx.cap(2).toInt();
            projectsStatus.append(JenkinsJob(name, buildNumber, status));
        }
    }
    emit statusUpdated(projectsStatus);
    qDebug()<<"done";
}
