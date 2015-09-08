#include "trayicon.h"
#include "configurationwindow.h"
#include "backend.h"
#include <QApplication>
#include <QResource>
#include <QMessageBox>
#include <QTimer>

int main(int argc, char** argv){
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Lorentz83");
    QCoreApplication::setOrganizationDomain("https://github.com/Lorentz83");
    QCoreApplication::setApplicationName("JenkinsTray");

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"), QObject::tr("Cannot detect any system tray."));
        return 1;
    }

    QApplication::setQuitOnLastWindowClosed(false);

    Configuration sharedConfig;
    ConfigurationWindow configuration(&sharedConfig);
    if ( argc > 1 ) {
        sharedConfig.setUrl(argv[1]);
        if (argc > 2) {
            sharedConfig.setRefreshSec(QString(argv[2]).toInt());
        }
        if (argc > 3) {
            QString ignore(argv[3]);
            sharedConfig.setIgnoreSslErrors(ignore.compare("t", Qt::CaseInsensitive) == 0 || ignore.compare("true", Qt::CaseInsensitive) == 0);
        }
    }

    TrayIcon tray(&configuration);
    Backend backend(&sharedConfig);
    QTimer timer;

    tray.show();
    backend.refresh();
    timer.start(sharedConfig.refreshMillisec());

    app.connect(&timer, &QTimer::timeout, &backend, &Backend::refresh);
    app.connect(&tray, &TrayIcon::configure, &configuration, &ConfigurationWindow::show);
    app.connect(&tray, &TrayIcon::refresh, &backend, &Backend::refresh);
    app.connect(&backend, &Backend::statusUpdated, &tray, &TrayIcon::updateStatus);

    app.connect(&configuration, &ConfigurationWindow::accepted, [&](){
        timer.start(sharedConfig.refreshMillisec());
        backend.refresh();
    });

    return app.exec();
}
