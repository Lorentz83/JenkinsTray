#include "trayicon.h"
#include "configurationwindow.h"
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

    ConfigurationWindow configuration;
    TrayIcon tray(&configuration);
    tray.show();

    app.connect(&tray, &TrayIcon::configure, &configuration, &ConfigurationWindow::show);

    return app.exec();
}
