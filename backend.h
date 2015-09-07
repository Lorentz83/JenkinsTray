#ifndef BACKEND_H
#define BACKEND_H

#include "configuration.h"
#include "jenkinsjob.h"

#include <QNetworkAccessManager>
#include <QVector>

class Backend : public QObject
{
    Q_OBJECT
    Configuration *_configuration;
    QNetworkAccessManager _netManager;

public:
    explicit Backend(Configuration *configuration, QObject *parent = 0);

signals:
    void statusUpdated(QVector<JenkinsJob>, QString);

private slots:
    void sslError(QNetworkReply *reply, const QList<QSslError> &errors);
    void netResponse(QNetworkReply* reply);

public slots:
    void refresh();
};

#endif // BACKEND_H
