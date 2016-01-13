#include "configuration.h"
#include <QSettings>

void Configuration::setUrl(QString url) {
    settings_.setValue("url", url);
}

QString Configuration::url() {
    return settings_.value("url", "http://localhost/jenkins").toString();
}

void Configuration::setRefreshSec(int refresh) {
    if (refresh <= 0)
        refresh = 1;
    settings_.setValue("refresh", refresh);
}

int Configuration::refreshSec() {
    return settings_.value("refresh", 10).toInt();
}

void Configuration::setIgnoreSslErrors(bool val) {
    settings_.setValue("ignoreSslErrors", val);
}

bool Configuration::ignoreSslErrors() {
    return settings_.value("ignoreSslErrors", false).toBool();
}

bool Configuration::firstRun() {
    if ( settings_.value("firstRun", true).toBool() ) {
        settings_.setValue("firstRun", false);
        return true;
    }
    return false;
}

void Configuration::setPlaySounds(bool val) {
    settings_.setValue("playSounds", val);
}

bool Configuration::playSounds() {
    return settings_.value("playSounds", true).toBool();
}
