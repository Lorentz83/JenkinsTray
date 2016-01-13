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
    Configuration *config_;
    Ui::ConfigurationWindow *ui_;
    QRegExp urlRegexp_;

public slots:
    virtual void accept();
    virtual void setVisible(bool visible);

public:
    explicit ConfigurationWindow(Configuration *config, QWidget *parent = 0);
    ~ConfigurationWindow();

};

#endif // CONFIGURATIONWINDOW_H
