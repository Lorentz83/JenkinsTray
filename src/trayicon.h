#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QSignalMapper>
#include <QSoundEffect>
#include <QUrl>
#include <QTemporaryDir>

#include "jenkinsjob.h"
#include "configuration.h"

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
    QMenu *_buildsMenu;
    QMap<JobStatus, QIcon> _icons;
    QSignalMapper _urlMapper;
    QSoundEffect _failSound, _successSound;
    QTemporaryDir *_soundDir;
    int _lastBrokenBuilds;
    Configuration *_config;
    QIcon appIcon;

protected slots:
    void openUrl(const QString& url);

public slots:
    void about();
    void updateStatus(const QVector<JenkinsJob>&, const QString&);

signals:
    void refresh();
    void configure();

public:
    TrayIcon(Configuration *config, QWidget *parent);
    ~TrayIcon();
};



#endif // TRAYICON_H
