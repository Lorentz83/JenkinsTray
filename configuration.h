#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QDialog>

class Configuration {
public:
    QString url;
    int refreshSec;

    Configuration();
    int refreshMillisec() {
        return refreshSec * 1000;
    }
};


#endif // CONFIGURATION_H
