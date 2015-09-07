#ifndef TRAYICON_H
#define TRAYICON_H

#include "jenkinsjob.h"
#include <QSystemTrayIcon>
#include <QMenu>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
    QMenu *_buildsMenu;
    QMap<JobStatus, QIcon> _icons;

public slots:
    void about();
    void updateStatus(const QVector<JenkinsJob>&);

signals:
    void refresh();
    void configure();

public:
    TrayIcon(QWidget *parent = NULL);
};



#endif // TRAYICON_H
