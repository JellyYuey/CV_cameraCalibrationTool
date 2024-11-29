#include "mainwindow.h"
#include "camera_calibration.h"
#include "image_processing.h"
#include "mylabel.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  // ui->actionExport_Camera_Parameters->setEnabled(false);
  // ui->actionCalilbrate->setEnabled(false);
  // ui->actionSettings->setEnabled(false);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::addPixmap(const QPixmap &img) {
  MyLabel *m = new MyLabel(img, ui->listWidget);
  connect(m, &MyLabel::sig_switch_img, this,
          [this](const QPixmap &img) { ui->label->setPixmap(img); });
  QListWidgetItem *item = new QListWidgetItem();
  item->setSizeHint(m->sizeHint());
  ui->listWidget->addItem(item);
  ui->listWidget->setItemWidget(item, m);
}

QString MainWindow::getCameraType() const { return cameraType; }

QString MainWindow::getCalibrationBoardType() const {
  return calibrationBoardType;
}

int MainWindow::getCalibrationBoardSize() const { return calibrationBoardSize; }

/**
 * @brief MainWindow::on_actionAdd_Images_triggered
 * add images
 */
void MainWindow::on_actionAdd_Images_triggered() {
  QStringList selectedFiles =
      QFileDialog::getOpenFileNames(nullptr, "Open Files", QDir::homePath());

  foreach (const QString &selectedFile, selectedFiles) {
    qDebug() << selectedFile;
    QPixmap pixmap(selectedFile);
    pixmap = pixmap.scaled(ui->label->size(), Qt::KeepAspectRatio,
                           Qt::SmoothTransformation);
    addPixmap(pixmap);
  }
  ui->actionCalilbrate->setEnabled(true);
  ui->actionSettings->setEnabled(true);
}

/**
 * @brief MainWindow::on_actionSettings_triggered
 * open setting page
 */
void MainWindow::on_actionSettings_triggered() {
  if (!settingsDialog) {
    settingsDialog = new Settings(this);
  }
  settingsDialog->exec();
}

void MainWindow::on_actionCalibrate_triggered() {}
