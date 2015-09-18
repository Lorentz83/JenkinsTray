#include "trayicon.h"
#include "configurationwindow.h"
#include "backend.h"
#include <QApplication>
#include <QResource>
#include <QMessageBox>
#include <QTimer>
#include <QThread>

int main(int argc, char** argv){
    QApplication app(argc, argv);
    app.setOrganizationName("Lorentz83");
    app.setOrganizationDomain("https://github.com/Lorentz83");
    app.setApplicationName("JenkinsTray");
    app.setWindowIcon(QIcon(":/ico/appicon"));

    int waitSystray = -1;
    if ( argc > 1 && QStringLiteral("-w") == argv[1] ) {
        waitSystray = 0;
        if ( argc > 2 ) {
            waitSystray = QString(argv[2]).toInt();
        }
    }

    if (waitSystray >=0) {
        for (int n = 0 ; !QSystemTrayIcon::isSystemTrayAvailable() && ( waitSystray == 0 || n < waitSystray) ; n++) {
            qWarning() << "waiting for system tray";
            QThread::sleep(1);
        }
    }

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"), QObject::tr("Cannot detect any system tray."));
        return 1;
    }

    QApplication::setQuitOnLastWindowClosed(false);

    Configuration sharedConfig;

    ConfigurationWindow configuration(&sharedConfig);
    TrayIcon tray(&sharedConfig, &configuration);
    Backend backend(&sharedConfig);
    QTimer timer;

    tray.show();
    if ( sharedConfig.firstRun() )
        configuration.show();

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
