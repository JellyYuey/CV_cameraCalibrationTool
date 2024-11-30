#include "settings.h"
#include "QSettings"
#include "ui_settings.h" // 包含生成的头文件
#include <QFileDialog>

Settings::Settings(QWidget *parent) : QDialog(parent), ui(new Ui::Settings) {
  ui->setupUi(this);
  loadSettings();
}

Settings::~Settings() { delete ui; }

void Settings::on_okButton_clicked() {
  saveSettings();
  accept();
}

void Settings::on_cancelButton_clicked() { reject(); }

void Settings::saveSettings() {
  QSettings settings("MyCompany", "MyApp");

  settings.setValue("CameraModel", ui->CameraModel->currentText());
  settings.setValue("Boardsize", ui->CalibrationBoardSize->currentText());
  settings.setValue("Boardtype", ui->CalibrationBoardType->currentText());
}

void Settings::loadSettings() {
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

// Add a public method in the Settings class to get the physical size of the
// grid
float Settings::getBoardSize() {
  QString boardSizeText = ui->CalibrationBoardSize->currentText();
  return boardSizeText.toFloat(); // Use the previously defined mapping method
}

// 实现 getCameraModel() 方法
QString Settings::getCameraModel() const {
  return ui->CameraModel->currentText();
}
