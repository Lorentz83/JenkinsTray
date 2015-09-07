#include "trayicon.h"
#include "configurationwindow.h"
#include "backend.h"
#include <QApplication>
#include <QResource>
#include <QMessageBox>
#include <QTimer>

int main(int argc, char** argv){
    QApplication app(argc, argv);
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"), QObject::tr("Cannot detect any system tray."));
        return 1;
    }

    QApplication::setQuitOnLastWindowClosed(false);

    Configuration sharedConfig;
    ConfigurationWindow configuration(&sharedConfig);
    if ( argc > 1 ) {
        sharedConfig.url = argv[1];
        if (argc > 2) {
            sharedConfig.refreshSec = QString(argv[2]).toInt();
            if (sharedConfig.refreshSec <= 0)
                sharedConfig.refreshSec = 1;
        }
        if (argc > 3) {
            QString ignore(argv[3]);
            sharedConfig.ignoreSslErrors = ignore.compare("t", Qt::CaseInsensitive) == 0 || ignore.compare("true", Qt::CaseInsensitive) == 0;
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
