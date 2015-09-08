#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QSignalMapper>
#include <QSoundEffect>
#include "jenkinsjob.h"
#include <QUrl>
#include <QTemporaryDir>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
    QMenu *_buildsMenu;
    QMap<JobStatus, QIcon> _icons;
    QSignalMapper _urlMapper;
    QSoundEffect _failSound, _successSound;
    QTemporaryDir *_soundDir;
    JobStatus _lastGlobalStatus;

protected slots:
    void openUrl(const QString& url);

public slots:
    void about();
    void updateStatus(const QVector<JenkinsJob>&, const QString&);

signals:
    void refresh();
    void configure();

public:
    TrayIcon(QWidget *parent = NULL);
    ~TrayIcon();
};



#endif // TRAYICON_H
