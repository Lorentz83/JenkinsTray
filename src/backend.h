#ifndef BACKEND_H
#define BACKEND_H

#include "configuration.h"
#include "jenkinsjob.h"

#include <QNetworkAccessManager>
#include <QVector>
#include <QIODevice>

class Backend : public QObject
{
    Q_OBJECT
    Configuration *_configuration;
    QNetworkAccessManager _netManager;

public:
    explicit Backend(Configuration *configuration, QObject *parent = 0);

signals:
    void statusUpdated(JenkinsStatus);

private slots:
    void sslError(QNetworkReply *reply, const QList<QSslError> &errors);
    void netResponse(QNetworkReply* reply);

public slots:
    /**
     * Loads the status from jenkins.
     * This method queries /rssAll
     */
    void loadStatus();

    /**
     * Refreshes the status of jenkins.
     * This method queries /rssLatest
     */
    void refresh();
};

JenkinsStatus parseJenkinsRss(QIODevice &rss);


#endif // BACKEND_H
