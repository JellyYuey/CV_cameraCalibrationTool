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

void MainWindow::on_actionCalibrate_triggered() {
  // 获取用户上传的图像
  QFileDialog dialog(this, "Select Images", "", "Images (*.jpg *.png)");
  dialog.setFileMode(QFileDialog::ExistingFiles);
  if (dialog.exec()) {
    QStringList fileNames = dialog.selectedFiles();

    // 图像读取与处理
    std::vector<cv::Mat> images;
    std::vector<std::vector<cv::Point2f>> imagePoints;

    // 读取图像并检测角点
    ImageProcessing imgProcessor;
    if (imgProcessor.processImages(images, imagePoints)) {
      // 获取世界坐标系中的点
      std::vector<cv::Point3f> worldPoints;
      imgProcessor.getWorldCoordinates(worldPoints);

      // 创建相机标定类
      CameraCalibration calib;
      calib.setCameraParameters(cameraType, 9, 6,
                                30.0f); // 例：棋盘格大小为 9x6，每个格子 30mm
      bool success = calib.calibrate(images, imagePoints, worldPoints);

      if (success) {
        // 标定成功，获取相机矩阵和畸变系数
        cv::Mat cameraMatrix = calib.getCameraMatrix();
        cv::Mat distCoeffs = calib.getDistortionCoefficients();
        // 可以在UI上显示相机标定结果
        // e.g., ui->cameraMatrixLabel->setText(cameraMatrix);
      } else {
        QMessageBox::warning(this, "Calibration", "Camera calibration failed.");
      }
    }
  }
}