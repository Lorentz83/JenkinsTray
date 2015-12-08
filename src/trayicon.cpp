#include "trayicon.h"

#include <QMessageBox>
#include <QDebug>
#include <QIcon>
#include <QDesktopServices>
#include <QDir>
#include <QStyle>
#include <QApplication>

TrayIcon::TrayIcon(Configuration *config, QWidget *parent) :
    QSystemTrayIcon(parent), _config(config), appIcon(parent->windowIcon()) {
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

    menu->addSeparator();
    action = menu->addAction(parent->style()->standardIcon(QStyle::SP_DialogCloseButton), tr("&Quit"));
    connect(action, &QAction::triggered, QApplication::instance(), &QApplication::quit);
    action->setMenuRole(QAction::QuitRole);

    connect(this, &TrayIcon::activated, [this](QSystemTrayIcon::ActivationReason reason){if (reason == QSystemTrayIcon::Trigger)openUrl(_config->url());});

    _soundDir = new QTemporaryDir();
    if (_soundDir->isValid()) {
        QDir tmpDir(_soundDir->path());
        QFile::copy(":/sounds/ko.wav", tmpDir.filePath("ko.wav"));
        QFile::copy(":/sounds/ok.wav", tmpDir.filePath("ok.wav"));
        _failSound.setSource(QUrl::fromLocalFile(tmpDir.filePath("ko.wav")));
        _successSound.setSource(QUrl::fromLocalFile(tmpDir.filePath("ok.wav")));
        _failSound.setLoopCount(1); _failSound.setCategory("notifications");
        _successSound.setLoopCount(1); _failSound.setCategory("notifications");
    }

    _lastBrokenBuilds = -1;
}

TrayIcon::~TrayIcon(){
    delete _soundDir;
}

void TrayIcon::openUrl(const QString& url) {
    qDebug() << "opening "<< url;
    QDesktopServices::openUrl(QUrl(url));
}

void TrayIcon::about() {
    QMessageBox box;
    box.setTextFormat(Qt::RichText);
    box.setWindowTitle(tr("About JenkinsTray"));
    box.setText(tr("© 2015, Lorenzo Bossi\n"
                   "This program is free software licensed under the <a href='http://www.gnu.org/licenses/gpl-3.0.html'>GPL v3</a> or later."
                   "<br/><br/>"
                   "To download the source code or report any bug visit<br/>"
                   "<a href='https://github.com/Lorentz83/JenkinsTray'>https://github.com/Lorentz83/JenkinsTray</a>"
                   "<br/><br/>"
                   "Icons by <a href='http://iconka.com'>http://iconka.com</a>"));
    box.setIconPixmap(appIcon.pixmap(128));
    box.exec();
}

void TrayIcon::updateStatus(const JenkinsStatus& status) {
    _buildsMenu->clear();

    if ( !status.isValid() ) {
        showMessage(tr("JenkinsTray error"), status.errorMessage(), QSystemTrayIcon::Critical);
        return;
    }

    QStringList brokenBuilds;
    JobStatus globalStatus = JobStatus::UNKNOWN;

    foreach( const JenkinsJob job, status.jobs() ) {
        QAction *action = _buildsMenu->addAction(_icons.value(job.status()), job.name() + ": " + toQString(job.status()));
        globalStatus = globalStatus && job.status();
        _urlMapper.setMapping(action, job.url());
        connect(action, SIGNAL(triggered(bool)), &_urlMapper, SLOT(map()));
        if ( job.status() == JobStatus::FAILURE )
            brokenBuilds.append(job.name());
    }

    _buildsMenu->setEnabled(!_buildsMenu->isEmpty());

    if (_lastBrokenBuilds >= 0) {
        if ( (globalStatus == JobStatus::SUCCESS || globalStatus == JobStatus::INSTABLE) && _lastBrokenBuilds ) {
            if ( _config->playSounds() )
                _successSound.play();
            showMessage(tr("All the projects are fixed!", "", status.jobs().size()), "", QSystemTrayIcon::Information);
        }
        if ( globalStatus == JobStatus::FAILURE && ( brokenBuilds.size() < _lastBrokenBuilds ) ) {
            showMessage(tr("%n projects were fixed", "", _lastBrokenBuilds - brokenBuilds.size()), "", QSystemTrayIcon::Warning);
        }
        if ( globalStatus == JobStatus::FAILURE && ( !_lastBrokenBuilds || brokenBuilds.size() > _lastBrokenBuilds ) ) {
            if ( _config->playSounds() )
                _failSound.play();
            showMessage(tr("%n projects are broken!", "", brokenBuilds.size()),
                        brokenBuilds.join("; "), QSystemTrayIcon::Critical);
        }
    }

    _lastBrokenBuilds = brokenBuilds.size();
    setIcon(_icons.value(globalStatus));
    setToolTip(tr("JenkinsTray: %n projects monitored", "", status.jobs().size()));
}
