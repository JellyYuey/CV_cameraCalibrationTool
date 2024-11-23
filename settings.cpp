#include "ui_settings.h"  // 包含生成的头文件
#include "QSettings"
#include "settings.h"

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
    QSettings settings("MyCompany", "MyApp");

    settings.setValue("CameraModel", ui->CameraModel->currentText());
    settings.setValue("Boardsize", ui->CalibrationBoardSize->currentText());
    settings.setValue("Boardtype", ui->CalibrationBoardType->currentText());
}

void Settings::loadSettings()
{
    QSettings settings("MyCompany", "MyApp");

    QString CameraModelLabel = settings.value("CameraModel").toString();
    if (!CameraModelLabel.isEmpty()) {
        int index = ui->CameraModel->findText(CameraModelLabel);
        if (index != -1) {
            ui->CameraModel->setCurrentIndex(index);
        }
    }

    QString BoardsizeLabel = settings.value("Boardsize").toString();
    if (!BoardsizeLabel.isEmpty()) {
        int index = ui->CalibrationBoardSize->findText(BoardsizeLabel);
        if (index != -1) {
            ui->CalibrationBoardSize->setCurrentIndex(index);
        }
    }

    QString BoardTypeLabel = settings.value("BoardType").toString();
    if (!BoardTypeLabel.isEmpty()) {
        int index = ui->CalibrationBoardType->findText(BoardTypeLabel);
        if (index != -1) {
            ui->CalibrationBoardType->setCurrentIndex(index);
        }
    }
}

