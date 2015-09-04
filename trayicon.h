#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public slots:
    void about();

signals:
    void refresh();
    void configure();

public:
    TrayIcon(QWidget *parent = NULL);
};



#endif // TRAYICON_H
