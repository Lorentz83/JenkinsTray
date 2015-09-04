#ifndef CONFIGURATIONWINDOW_H
#define CONFIGURATIONWINDOW_H

#include <QRegExp>

#include "configuration.h"
#include "ui_ConfigurationWindow.h"

namespace Ui {
class ConfigurationWindow;
}

class ConfigurationWindow : public QDialog
{
    Q_OBJECT

public slots:
    virtual void accept();
    virtual void setVisible(bool visible);

public:
    explicit ConfigurationWindow(QWidget *parent = 0);
    ~ConfigurationWindow();
    Configuration config;

private:
    Ui::ConfigurationWindow *ui;
    QRegExp urlRegexp;
};

#endif // CONFIGURATIONWINDOW_H
