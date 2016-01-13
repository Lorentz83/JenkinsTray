#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QSignalMapper>
#include <QSoundEffect>
#include <QUrl>
#include <QMap>
#include <QTemporaryDir>

#include "jenkinsjob.h"
#include "configuration.h"

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

    QSignalMapper urlMapper_;
    QSoundEffect _failSound, successSound_;
    QTemporaryDir *soundDir_;
    Configuration *config_;
    QIcon appIcon_;
    QMenu *buildsMenu_;
    QMap<JobStatus, QIcon> icons_;
    QMap<QString, JobStatus> oldStatus_;
    bool lastUpdateWasError_;
    QString lastErrorMessage_;

protected slots:
    void openUrl(const QString& url);

public slots:
    void about();
    void updateStatus(const JenkinsStatus&);

signals:
    void refresh();
    void configure();

public:
    TrayIcon(Configuration *config, QWidget *parent);
    ~TrayIcon();
};



#endif // TRAYICON_H
