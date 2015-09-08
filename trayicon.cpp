#include "trayicon.h"

#include <QMessageBox>
#include <QDebug>
#include <QIcon>
#include <QDesktopServices>
#include <QDir>
#include <QStyle>


TrayIcon::TrayIcon(Configuration *config, QWidget *parent) :
    QSystemTrayIcon(parent), _config(config) {
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

    action = menu->addAction(parent->style()->standardIcon(QStyle::SP_BrowserReload), tr("&Refresh"));
    connect(action, &QAction::triggered, this, &TrayIcon::refresh);

    action = menu->addAction(tr("&Configure…"));
    connect(action, &QAction::triggered, this, &TrayIcon::configure);

    action = menu->addAction(tr("&About…"));
    connect(action, &QAction::triggered, this, &TrayIcon::about);

    menu->addSeparator();
    _buildsMenu = menu->addMenu("builds");
    setContextMenu(menu);

    connect(&_urlMapper, SIGNAL(mapped(QString)), this, SLOT(openUrl(QString)));

    _soundDir = new QTemporaryDir();
    if (_soundDir->isValid()) {
        QDir tmpDir(_soundDir->path());
        QFile::copy(":/sounds/ko.wav", tmpDir.filePath("ko.wav"));
        QFile::copy(":/sounds/ok.wav", tmpDir.filePath("ok.wav"));
        _failSound.setSource(QUrl::fromLocalFile(tmpDir.filePath("ko.wav")));
        _successSound.setSource(QUrl::fromLocalFile(tmpDir.filePath("ok.wav")));
        _failSound.setLoopCount(1); _failSound.setVolume(.25f);
        _successSound.setLoopCount(1); _successSound.setVolume(.25f);
    }

    _lastGlobalStatus = JobStatus::UNKNOWN;
}

TrayIcon::~TrayIcon(){
    delete _soundDir;
}

void TrayIcon::openUrl(const QString& url) {
    qDebug() << "opening "<< url;
    QDesktopServices::openUrl(QUrl(url));
}

void TrayIcon::about() {
    QWidget *p = (QWidget *)parent();
    QMessageBox box(p);
    box.setTextFormat(Qt::RichText);
    box.setWindowTitle(tr("About JenkinsTray"));
    box.setText(tr("© 2015, Lorenzo Bossi\n"
                   "This program is free software licensed under the <a href='http://www.gnu.org/licenses/gpl-3.0.html'>GPL v3</a> or later."
                   "<br/><br/>"
                   "To download the source code or report any bug visit<br/>"
                   "<a href='https://github.com/Lorentz83/JenkinsTray'>https://github.com/Lorentz83/JenkinsTray</a>"
                   "<br/><br/>"
                   "Icons by <a href='http://iconka.com'>http://iconka.com</a>"));
    box.setIconPixmap(p->windowIcon().pixmap(128));
    box.exec();
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


    if ( _config->playSounds() && (globalStatus == JobStatus::SUCCESS || globalStatus == JobStatus::INSTABLE) && _lastGlobalStatus == JobStatus::FAILURE) {
        _successSound.play();
    }
    if (_config->playSounds() && globalStatus == JobStatus::FAILURE && _lastGlobalStatus != JobStatus::FAILURE) {
        _failSound.play();
    }

    _lastGlobalStatus = globalStatus;
    setIcon(_icons.value(globalStatus));
}
