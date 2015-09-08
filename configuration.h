#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QSettings>

class Configuration {    
    QSettings _settings;

public:
    int refreshMillisec() {
        return refreshSec() * 1000;
    }

    bool firstRun();

    void setUrl(QString url);
    QString url();
    void setRefreshSec(int refresh);
    int refreshSec();
    void setIgnoreSslErrors(bool val);
    bool ignoreSslErrors();
    void setPlaySounds(bool val);
    bool playSounds();
};


#endif // CONFIGURATION_H
