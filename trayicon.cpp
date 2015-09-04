#include "trayicon.h"

#include <QMessageBox>

TrayIcon::TrayIcon(QWidget *parent) : QSystemTrayIcon(parent)
{
    setIcon(QIcon(":/ico/fog"));
    QMenu *menu = new QMenu(parent);
    QAction *action;

    action = menu->addAction(tr("&Refresh"));
    connect(action, &QAction::triggered, this, &TrayIcon::refresh);

    action = menu->addAction(tr("&Configure…"));
    connect(action, &QAction::triggered, this, &TrayIcon::configure);

    action = menu->addAction(tr("&About…"));
    connect(action, &QAction::triggered, this, &TrayIcon::about);

    setContextMenu(menu);
}


void TrayIcon::about() {
    QMessageBox::about((QWidget *)parent(), tr("About JenkinsTray"), tr("© 2015, Lorenzo Bossi\n"
                                                               "This program is free software licensed under the GPL v3 or later\n"
                                                               "Icons are provided by http://iconka.com"));
}
