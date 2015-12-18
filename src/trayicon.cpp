#include "trayicon.h"

#include <QMessageBox>
#include <QDebug>
#include <QIcon>
#include <QDesktopServices>
#include <QDir>
#include <QStyle>
#include <QApplication>

TrayIcon::TrayIcon(Configuration *config, QWidget *parent) :
    QSystemTrayIcon(parent), _config(config), _appIcon(parent->windowIcon()) {
    _icons = QMap<JobStatus, QIcon>{
        {JobStatus::UNKNOWN, QIcon(":/ico/unknown")},
        {JobStatus::RUNNING, QIcon(":/ico/running")},
        {JobStatus::INSTABLE, QIcon(":/ico/instable")},
        {JobStatus::SUCCESS, QIcon(":/ico/success")},
        {JobStatus::FAILURE, QIcon(":/ico/failure")},
    };
    setIcon(_appIcon);
    _lastUpdateWasError = false;

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
    box.setIconPixmap(_appIcon.pixmap(128));
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
    static bool firstRun = true;

    _buildsMenu->clear();

    if ( !status.isValid() ) {
        // if there is a new error message
        // or there is an error without a message
        //   show a notification
        if ( _lastErrorMessage != status.errorMessage() || _lastUpdateWasError == false ) {
            showMessage(tr("JenkinsTray error"), status.errorMessage(), QSystemTrayIcon::Critical);
            setToolTip(tr("JenkinsTray error: %1").arg(status.errorMessage()));
            setIcon(_appIcon);
            _lastErrorMessage = status.errorMessage();
        }
        _lastUpdateWasError = true;
        return;
    } else {
        _lastErrorMessage = "";
        _lastUpdateWasError = false;
    }

    QStringList brokenBuilds;
    QStringList newBuilds;
    int newBroken = 0, newFixed = 0, probablyStillBroken = 0;
    int running = 0, successful = 0;
    JobStatus globalStatus = JobStatus::UNKNOWN;

    QSet<QString> activeJobs;
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
                if (_oldStatus.value(job.name(), JobStatus::UNKNOWN) == JobStatus::UNKNOWN)
                    newBuilds.append(job.name());
                if (_oldStatus.value(job.name(), JobStatus::UNKNOWN) == JobStatus::FAILURE)
                    newFixed++;
                successful++;
                break;
            case JobStatus::RUNNING:
                if (_oldStatus.value(job.name(), JobStatus::UNKNOWN) == JobStatus::FAILURE)
                    probablyStillBroken++;
                running++;
                break;
            default:
                break;
        }
        if (job.status() == JobStatus::FAILURE || job.status() == JobStatus::INSTABLE|| job.status() == JobStatus::SUCCESS)
            _oldStatus.insert(job.name(), job.status());
        activeJobs.insert(job.name());
    }
    // a project can be deleted, this is to remove its status.
    for (auto old = _oldStatus.begin() ; old != _oldStatus.end() ;) {
        if (!activeJobs.contains(old.key()))
            old = _oldStatus.erase(old);
        else
            old++;
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
            QString msg = tr("%n newly broken project(s)", "", newBroken);
            if ( newFixed > 0 )
                msg += tr(" and %n newly fixed project(s)", "", newFixed);
            showMessage(msg, tr("Broken project(s):\n%1", "", brokenBuilds.size()).arg(brokenBuilds.join(";\n")), QSystemTrayIcon::Critical);
        }
    }

    if ( !newBuilds.empty() && !firstRun) {
        showMessage(tr("New project(s)"), newBuilds.join(";\n"), QSystemTrayIcon::Information);
    }
    firstRun = false;

    QStringList tooltip;
    tooltip.append("JenkinsTray:");
    if (brokenBuilds.size() > 0) {
        tooltip.append(tr("%n project(s) broken;", "", brokenBuilds.size()));
    }
    if (running > 0) {
        tooltip.append(tr("%n project(s) running;", "", running));
    }
    if (successful > 0) {
        tooltip.append(tr("%n project(s) successfully built;", "", successful));
    }
    if (tooltip.size() == 1) {
        tooltip.append("no project monitored.");
        setIcon(_appIcon);
    }
    else {
        setIcon(_icons.value(globalStatus));
    }
    setToolTip(tooltip.join("\n"));
}
