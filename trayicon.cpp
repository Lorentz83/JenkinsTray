#include "trayicon.h"

#include <QMessageBox>
#include <qdebug.h>
#include <QMap>
#include <QIcon>
#include <QDesktopServices>
#include <QUrl>

TrayIcon::TrayIcon(QWidget *parent) : QSystemTrayIcon(parent)
{
    _icons = QMap<JobStatus, QIcon>{
        {JobStatus::UNKNOWN, QIcon(":/ico/unknown")},
        {JobStatus::RUNNING, QIcon(":/ico/running")},
        {JobStatus::INSTABLE, QIcon(":/ico/instable")},
        {JobStatus::SUCCESS, QIcon(":/ico/success")},
        {JobStatus::FAILURE, QIcon(":/ico/failure")},
    };

    setIcon(QIcon(":/ico/fog"));
    QMenu *menu = new QMenu(parent);
    QAction *action;

    action = menu->addAction(tr("&Refresh"));
    connect(action, &QAction::triggered, this, &TrayIcon::refresh);

    action = menu->addAction(QIcon(":/ico/sun"), tr("&Configure…"));
    connect(action, &QAction::triggered, this, &TrayIcon::configure);

    action = menu->addAction(tr("&About…"));
    connect(action, &QAction::triggered, this, &TrayIcon::about);

    menu->addSeparator();
    _buildsMenu = menu->addMenu("builds");
    setContextMenu(menu);

    connect(&_urlMapper, SIGNAL(mapped(QString)), this, SLOT(openUrl(QString)));

}

void TrayIcon::openUrl(const QString& url) {
    qDebug() << "opening "<< url;
    QDesktopServices::openUrl(QUrl(url));
}

void TrayIcon::about() {
    QMessageBox::about((QWidget *)parent(), tr("About JenkinsTray"), tr("© 2015, Lorenzo Bossi\n"
                                                               "This program is free software licensed under the GPL v3 or later\n"
                                                               "To download the source code or report any bug visit\n"
                                                               "https://github.com/Lorentz83/JenkinsTray\n"
                                                               "Icons by http://iconka.com"));
}

void TrayIcon::updateStatus(const QVector<JenkinsJob> &projects, const QString &message) {
    setToolTip(message);

    _buildsMenu->clear();

    JobStatus globalStatus = JobStatus::UNKNOWN;

    foreach(const JenkinsJob job, projects) {
        QAction *action = _buildsMenu->addAction(_icons.value(job.status), job.name + ": " + toQString(job.status));
        globalStatus = globalStatus && job.status;
        _urlMapper.setMapping(action, job.url);
        connect(action, SIGNAL(triggered(bool)), &_urlMapper, SLOT(map()));

    }

    _buildsMenu->setEnabled(!_buildsMenu->isEmpty());

    setIcon(_icons.value(globalStatus));
}
