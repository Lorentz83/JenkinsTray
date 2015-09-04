#include "configurationwindow.h"

#include "ui_configuration.h"

ConfigurationWindow::ConfigurationWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigurationWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/ico/appicon"));

    urlRegexp.setPattern("^https?://.*");
    urlRegexp.setCaseSensitivity(Qt::CaseInsensitive);
    ui->_url->setValidator(new QRegExpValidator(urlRegexp, this));
}

ConfigurationWindow::~ConfigurationWindow()
{
    delete ui;
}

void ConfigurationWindow::accept() {
    config.refreshSec = ui->_refresh->value();
    config.url = ui->_url->text();
    QDialog::accept();
}

void ConfigurationWindow::setVisible(bool visible) {
    if (visible) {
        ui->_refresh->setValue(config.refreshSec);
        ui->_url->setText(config.url);
    }
    QDialog::setVisible(visible);
}
