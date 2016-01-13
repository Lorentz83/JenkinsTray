#include "configurationwindow.h"


ConfigurationWindow::ConfigurationWindow(Configuration *config, QWidget *parent) :
    QDialog(parent),
    config_(config),
    ui_(new Ui::ConfigurationWindow) {
    ui_->setupUi(this);

    urlRegexp_.setPattern("^https?://.*");
    urlRegexp_.setCaseSensitivity(Qt::CaseInsensitive);
    ui_->url_->setValidator(new QRegExpValidator(urlRegexp_, this));
}

ConfigurationWindow::~ConfigurationWindow()
{
    delete ui_;
}

void ConfigurationWindow::accept() {
    config_->setRefreshSec(ui_->refresh_->value());
    config_->setUrl(ui_->url_->text());
    config_->setIgnoreSslErrors(ui_->ignoreSslErrors_->isChecked());
    config_->setPlaySounds(ui_->playSounds_->isChecked());
    QDialog::accept();
}

void ConfigurationWindow::setVisible(bool visible) {
    if (visible) {
        ui_->refresh_->setValue(config_->refreshSec());
        ui_->url_->setText(config_->url());
        ui_->ignoreSslErrors_->setChecked(config_->ignoreSslErrors());
        ui_->playSounds_->setChecked(config_->playSounds());
    }
    QDialog::setVisible(visible);
}
