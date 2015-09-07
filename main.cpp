#include "trayicon.h"
#include "configurationwindow.h"
#include "backend.h"
#include <QApplication>
#include <QResource>
#include <QMessageBox>

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
    }

    TrayIcon tray(&configuration);
    Backend backend(&sharedConfig);

    tray.show();
    backend.refresh();

    app.connect(&configuration, &ConfigurationWindow::accepted, &backend, &Backend::refresh);
    app.connect(&tray, &TrayIcon::configure, &configuration, &ConfigurationWindow::show);
    app.connect(&tray, &TrayIcon::refresh, &backend, &Backend::refresh);
    app.connect(&backend, &Backend::statusUpdated, &tray, &TrayIcon::updateStatus);

    return app.exec();
}
