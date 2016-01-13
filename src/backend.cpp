#include "backend.h"
#include <QNetworkReply>
#include <QTextCodec>
#include <QXmlQuery>
#include <QXmlResultItems>
#include <QBuffer>
#include <QRegExp>
#include <QMap>

Backend::Backend(Configuration *configuration, QObject *parent) :
    QObject(parent),
    configuration_(configuration),
    netManager_(parent)
{

    connect(&netManager_, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(netResponse(QNetworkReply*)));

    connect(&netManager_, &QNetworkAccessManager::sslErrors,
            this, &Backend::sslError);

}

void Backend::loadStatus() {
    QUrl url;
    if ( configuration_->url().endsWith('/') )
        url = QUrl(configuration_->url() + "rssAll" );
    else
        url = QUrl(configuration_->url() + "/rssAll" );
    netManager_.get(QNetworkRequest(url));
}

void Backend::refresh() {
    QUrl url;
    if ( configuration_->url().endsWith('/') )
        url = QUrl(configuration_->url() + "rssLatest" );
    else
        url = QUrl(configuration_->url() + "/rssLatest" );
    netManager_.get(QNetworkRequest(url));
}

void Backend::netResponse(QNetworkReply* reply){
    if (reply->error() != QNetworkReply::NoError) {
        emit statusUpdated(JenkinsStatus(reply->errorString()));
        return;
    }
    QUrl possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if ( !possibleRedirectUrl.isEmpty() && possibleRedirectUrl != reply->url() ) { //redirect
        qDebug() << "Http request redirected to " << possibleRedirectUrl;
        netManager_.get(QNetworkRequest(possibleRedirectUrl));
        return;
    }

    QByteArray encoded = reply->readAll();
    reply->deleteLater();

    QBuffer buffer(&encoded); // This is a QIODevice.
    buffer.open(QIODevice::ReadOnly);

    emit statusUpdated(parseJenkinsRss(buffer));
}

void Backend::sslError(QNetworkReply *reply, const QList<QSslError> &) {
    if (configuration_->ignoreSslErrors())
        reply->ignoreSslErrors();
}



JenkinsStatus parseJenkinsRss(QIODevice &rss) {
    QRegExp rx("^(.+) #([0-9]+) \\((.*)\\) (http.*)$");

    QXmlQuery queryEntry;
    queryEntry.bindVariable("rss", &rss);
    queryEntry.setQuery("declare default element namespace \"http://www.w3.org/2005/Atom\"; "
                   "declare variable $rss external; "
                   "for $entry in doc($rss)/feed/entry "
                   "return fn:concat($entry/title/string(), ' ', $entry/link[@type='text/html']/@href)");

    QStringList projects;
    if ( !queryEntry.evaluateTo(&projects) ) {
        QString error = QObject::tr("Not a jenkins reply");
        return JenkinsStatus(error);
    }

    QMap<QString, JenkinsJob> jobs;
    foreach (QString str, projects) {
        if (rx.indexIn(str) == -1) {
            qDebug() << "error parsing " << str;
        } else {
            QString name = rx.cap(1);
            int buildNumber = rx.cap(2).toInt();
            JobStatus status = parseJobStatus(rx.cap(3));
            QString url = rx.cap(4);

            JenkinsJob old = jobs.value(name);
            if ( !old.isValid() || old.buildNumber() < buildNumber ) {
                jobs.insert(name, JenkinsJob(name,buildNumber,status, url));
            }
        }
    }
    return JenkinsStatus(jobs.values());
}
