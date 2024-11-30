#include "mainwindow.h"
#include "calibrationresultviewer.h"
#include "cameracalibration.h"
#include "mylabel.h"
#include "ui_mainwindow.h"
#include "worldcoordinatecalculator.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      settingsDialog(new Settings(this)),
      calibration() // 初始化 calibration 对象
{
  ui->setupUi(this);

  // 初始化默认参数
  calculator.setCols(9);
  calculator.setRows(6);
  calculator.setBoxSize(25.0f);
}

MainWindow::~MainWindow() {
  delete settingsDialog;
  delete ui;
}

/**
 * @brief MainWindow::on_actionAdd_Images_triggered
 * Add images
 */
void MainWindow::addPixmap(const QPixmap &img) {

  int rows = 6; // Number of rows in the chessboard pattern
  int cols = 9; // Number of columns in the chessboard pattern
  float boxSize =
      settingsDialog
          ->getBoardSize(); // Physical size of each grid, in millimeters

  calculator.setRows(rows);
  calculator.setCols(cols);
  calculator.setBoxSize(boxSize);

  // Convert QPixmap to QImage
  QImage qImage = img.toImage();

  // Check if QImage is null
  if (qImage.isNull()) {
    qDebug() << "QImage is null!";
    return;
  }

  // Call the image processing method
  std::vector<cv::Point2f> corners;
  bool found = calibration.processImage(qImage, corners);

  // If corners are detected, display the results
  if (found) {

    // Calculate world coordinates
    std::vector<cv::Point3f> worldCorners =
        calculator.generateWorldCoordinates(corners);

    // 保存图像点和世界点
    allImagePoints.emplace_back(corners);
    allObjectPoints.emplace_back(worldCorners);

    // Convert the processed Mat (with drawn corners) to QImage
    cv::Mat processedMat =
        calibration.getProcessedImage(); // Get the image with drawn corners

    QImage processedImage =
        calibration.MatToQImage(processedMat); // Convert to QImage

    QPixmap processedPixmap =
        QPixmap::fromImage(processedImage); // Convert to QPixmap

    // Display the processed image
    MyLabel *m = new MyLabel(processedPixmap, ui->listWidget);
    connect(m, &MyLabel::sig_switch_img, this,
            [this](const QPixmap &img) { ui->label->setPixmap(img); });

    // Add the image to QListWidget
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(m->sizeHint());
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, m);
    qDebug() << "Image added to QListWidget";
  } else {
    qDebug() << "Corners not found in image";
  }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
  QPixmap pixmap = ui->label->pixmap();
  auto hashValue = calibration.customPixmapHash(pixmap);
  // 打印哈希值
  qDebug() << "匹配图像Pixmap Hash: " << hashValue;
  // 获取当前图像的特征点
  std::vector<cv::Point2f> featurePoints = calibration.getFeaturePoints(pixmap);

  if (event->button() == Qt::LeftButton) {
    // 获取相对于 ui->label 的坐标
    QPoint labelPos = ui->label->mapFromGlobal(event->globalPos());

    // 计算缩放比例
    qreal scaleX = (qreal)ui->label->width() / (qreal)pixmap.width();
    qreal scaleY = (qreal)ui->label->height() / (qreal)pixmap.height();

    // 将点击位置从 label 坐标系转换为图像的像素坐标
    QPoint imagePos = QPoint(labelPos.x() / scaleX, labelPos.y() / scaleY);

    // 设置一个容忍度范围 (根据需要调整此值)
    const int tolerance = 10; // 10 pixels tolerance

    // 遍历特征点，判断点击位置是否在特征点附近
    for (const auto &pt : featurePoints) {
      // 将特征点从 cv::Point2f 转换为 QPoint
      QPoint featurePoint(pt.x, pt.y);

      // 判断点击位置是否与特征点位置接近
      if ((imagePos - featurePoint).manhattanLength() <= tolerance) {
        qDebug() << "Mouse clicked near a feature point at: " << featurePoint;
        // 处理点击事件
        // 例如，你可以标记该特征点，或者执行其他操作
        break; // 如果找到一个特征点就退出
      }
    }
  } else {
    qDebug() << "No feature points found for this image.";
  }
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
  ui->actionCalibrate->setEnabled(true);
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

  // 获取棋盘格参数
  int rows = calculator.getRows();
  int cols = calculator.getCols();
  float boxSize = calculator.getBoxSize();

  // 从 Settings 中获取相机类型
  if (settingsDialog) {
    QString selectedCameraType = settingsDialog->getCameraModel();
    if (!selectedCameraType.isEmpty()) {
      cameraType = selectedCameraType;
    } else {
      cameraType = "Standard"; // 设置默认值
    }
  } else {
    cameraType = "Standard"; // 设置默认值
  }

  qDebug() << "Selected camera type:" << cameraType;

  // 调用标定函数，并传递参数
  if (calibration.calibrate(allImagePoints, allObjectPoints, cameraType)) {
    // 获取标定结果
    cv::Mat cameraMatrix = calibration.getCameraMatrix();
    cv::Mat distCoeffs = calibration.getDistCoeffs();
    double reprojectionError = calibration.getReprojectionError();

    // 创建并展示 CalibrationResultViewer
    CalibrationResultViewer viewer(cameraMatrix, distCoeffs, reprojectionError,
                                   this);
    viewer.exec();

  } else {
    QMessageBox::warning(
        this, tr("标定失败"),
        tr("相机标定失败，请检查数据是否完整或相机类型是否正确。"));
  }
}