#include "ui_settings.h"  // 包含生成的头文件
#include "settings.h"
#include "settingsStore.h"

Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);
    loadSettings();
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_okButton_clicked()
{
    saveSettings();
    accept();
}

void Settings::on_cancelButton_clicked()
{
    reject();
}

void Settings::saveSettings()
{
    SettingsStore& settings = SettingsStore::getInstance();
    settings.checkerBoardWidth = ui->checkerBoardWidth->value();
    settings.checkerBoardLength = ui->checkerBoardLength->value();
    settings.cameraModel = ui->cameraModel->currentText();
    settings.squareSize = ui->squareSize->value();
}

void Settings::loadSettings()
{
    SettingsStore& settings = SettingsStore::getInstance();
    ui->checkerBoardLength->setValue(settings.checkerBoardLength);
    ui->checkerBoardWidth->setValue(settings.checkerBoardWidth);
    ui->squareSize->setValue(settings.squareSize);
    ui->cameraModel->setCurrentIndex(ui->cameraModel->findText(settings.cameraModel));
}



