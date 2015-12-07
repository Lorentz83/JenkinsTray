#include "configurationwindow.h"


ConfigurationWindow::ConfigurationWindow(Configuration *config, QWidget *parent) :
    QDialog(parent),
    _config(config),
    ui(new Ui::ConfigurationWindow) {
    ui->setupUi(this);

    _urlRegexp.setPattern("^https?://.*");
    _urlRegexp.setCaseSensitivity(Qt::CaseInsensitive);
    ui->_url->setValidator(new QRegExpValidator(_urlRegexp, this));
}

ConfigurationWindow::~ConfigurationWindow()
{
    delete ui;
}

void ConfigurationWindow::accept() {
    _config->setRefreshSec(ui->_refresh->value());
    _config->setUrl(ui->_url->text());
    _config->setIgnoreSslErrors(ui->_ignoreSslErrors->isChecked());
    _config->setPlaySounds(ui->_playSounds->isChecked());
    QDialog::accept();
}

void ConfigurationWindow::setVisible(bool visible) {
    if (visible) {
        ui->_refresh->setValue(_config->refreshSec());
        ui->_url->setText(_config->url());
        ui->_ignoreSslErrors->setChecked(_config->ignoreSslErrors());
        ui->_playSounds->setChecked(_config->playSounds());
    }
    QDialog::setVisible(visible);
}