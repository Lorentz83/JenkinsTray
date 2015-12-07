#include "configuration.h"
#include <QSettings>

void Configuration::setUrl(QString url) {
    _settings.setValue("url", url);
}

QString Configuration::url() {
    return _settings.value("url", "http://localhost/jenkins").toString();
}

void Configuration::setRefreshSec(int refresh) {
    if (refresh <= 0)
        refresh = 1;
    _settings.setValue("refresh", refresh);
}

int Configuration::refreshSec() {
    return _settings.value("refresh", 10).toInt();
}

void Configuration::setIgnoreSslErrors(bool val) {
    _settings.setValue("ignoreSslErrors", val);
}

bool Configuration::ignoreSslErrors() {
    return _settings.value("ignoreSslErrors", false).toBool();
}

bool Configuration::firstRun() {
    if ( _settings.value("firstRun", true).toBool() ) {
        _settings.setValue("firstRun", false);
        return true;
    }
    return false;
}

void Configuration::setPlaySounds(bool val) {
    _settings.setValue("playSounds", val);
}

bool Configuration::playSounds() {
    return _settings.value("playSounds", true).toBool();
}