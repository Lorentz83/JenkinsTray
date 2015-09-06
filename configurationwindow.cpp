#include "configurationwindow.h"


ConfigurationWindow::ConfigurationWindow(Configuration *config, QWidget *parent) :
    QDialog(parent),
    _config(config),
    ui(new Ui::ConfigurationWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/ico/appicon"));

    _urlRegexp.setPattern("^https?://.*");
    _urlRegexp.setCaseSensitivity(Qt::CaseInsensitive);
    ui->_url->setValidator(new QRegExpValidator(_urlRegexp, this));
}

ConfigurationWindow::~ConfigurationWindow()
{
    delete ui;
}

void ConfigurationWindow::accept() {
    _config->refreshSec = ui->_refresh->value();
    _config->url = ui->_url->text();
    QDialog::accept();
}

void ConfigurationWindow::setVisible(bool visible) {
    if (visible) {
        ui->_refresh->setValue(_config->refreshSec);
        ui->_url->setText(_config->url);
    }
    QDialog::setVisible(visible);
}
