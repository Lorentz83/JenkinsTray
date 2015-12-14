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

JobStatus getStatus(const JenkinsStatus& status, const QString& jobName) {
    foreach ( const JenkinsJob& job, status.jobs() ) {
        if ( job.name() == jobName )
            return job.status();
    }
    return JobStatus::UNKNOWN;
}

void TrayIcon::updateStatus(const JenkinsStatus& status) {
    _buildsMenu->clear();

    if ( !status.isValid() ) {
        showMessage(tr("JenkinsTray error"), status.errorMessage(), QSystemTrayIcon::Critical);
        return;
    }

    QStringList brokenBuilds;
    int newBroken = 0, newFixed = 0, probablyStillBroken = 0;
    JobStatus globalStatus = JobStatus::UNKNOWN;

    foreach ( const JenkinsJob& job, status.jobs() ) {
        QAction *action = _buildsMenu->addAction(_icons.value(job.status()), job.name() + ": " + toQString(job.status()));
        globalStatus = globalStatus && job.status();
        _urlMapper.setMapping(action, job.url());
        connect(action, SIGNAL(triggered(bool)), &_urlMapper, SLOT(map()));
        switch( job.status() ) {
            case JobStatus::FAILURE:
                if ( _oldStatus.value(job.name(), JobStatus::UNKNOWN) != JobStatus::FAILURE)
                    newBroken++;
                brokenBuilds.append(job.name());
                break;
            case JobStatus::SUCCESS:
            case JobStatus::INSTABLE:
                if (_oldStatus.value(job.name(), JobStatus::UNKNOWN) == JobStatus::FAILURE)
                    newFixed++;
                break;
            case JobStatus::RUNNING:
                if (_oldStatus.value(job.name(), JobStatus::UNKNOWN) == JobStatus::FAILURE)
                    probablyStillBroken++;
                break;
            default:
                break;
        }
        if (job.status() == JobStatus::FAILURE || job.status() == JobStatus::INSTABLE|| job.status() == JobStatus::SUCCESS)
            _oldStatus.insert(job.name(), job.status());
    }

    _buildsMenu->setEnabled(!_buildsMenu->isEmpty());

    if (newFixed > 0 && brokenBuilds.size() == 0 && probablyStillBroken == 0) {
        if ( _config->playSounds() )
            _successSound.play();
        showMessage(tr("All the projects are fixed!", "", status.jobs().size()), "", QSystemTrayIcon::Information);
    } else {
        if ( newBroken > 0 ) {
            if ( _config->playSounds() )
                _failSound.play();
            QString msg = tr("%n new projects are broken", "", newBroken);
            if ( newFixed > 0 )
                showMessage(msg + tr(" and %n new projects are fixed", "", newFixed), "", QSystemTrayIcon::Critical);
            else
                showMessage(tr("%n projects are broken!", "", brokenBuilds.size()), brokenBuilds.join("; "), QSystemTrayIcon::Critical);
        }
    }
    setIcon(_icons.value(globalStatus));
    setToolTip(tr("JenkinsTray: %n projects monitored", "", status.jobs().size()));
}
