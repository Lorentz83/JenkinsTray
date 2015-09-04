#ifndef BACKEND_H
#define BACKEND_H

#include "configuration.h"

class Backend : public QObject
{
    Q_OBJECT
    Configuration *_configuration;

public:
    explicit Backend(Configuration *configuration, QObject *parent = 0);

signals:

public slots:
    void refresh();
};

#endif // BACKEND_H
