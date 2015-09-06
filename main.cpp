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

    Configuration config;
    ConfigurationWindow configuration(&config);
    if ( argc > 1 ) {
        config.url = argv[1];
    }

    TrayIcon tray(&configuration);
    Backend backend(&config);

    tray.show();
    backend.refresh();

    app.connect(&tray, &TrayIcon::configure, &configuration, &ConfigurationWindow::show);
    app.connect(&tray, &TrayIcon::refresh, &backend, &Backend::refresh);

    return app.exec();
}
