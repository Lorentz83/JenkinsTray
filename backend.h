#ifndef BACKEND_H
#define BACKEND_H

#include "configuration.h"
#include <QNetworkAccessManager>

class Backend : public QObject
{
    Q_OBJECT
    Configuration *_configuration;
    QNetworkAccessManager _netManager;

public:
    explicit Backend(Configuration *configuration, QObject *parent = 0);

signals:

private slots:
    void netResponse(QNetworkReply* reply);

public slots:
    void refresh();
};

#endif // BACKEND_H
