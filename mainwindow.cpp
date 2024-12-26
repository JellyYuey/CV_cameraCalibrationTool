#include "mainwindow.h"
#include "calibrationresultviewer.h"
#include "cameracalibration.h"
#include "mylabel.h"
#include "ui_mainwindow.h"
#include "worldcoordinatecalculator.h"

#include <QDir>
#include <QFileDialog>
#include <QImage>
#include <QMenu>
#include <QMessageBox>
#include <QPixmap>
#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      settingsDialog(new Settings(this)),
      calibration() // 初始化 calibration 对象
{
  ui->setupUi(this);
  setWindowTitle(tr("相机标定工具"));

  // 注释或移除默认参数设置
  // calculator.setCols(2);
  // calculator.setRows(2);
  // calculator.setBoxSize(20.0f);
  // imageProcessor.setBoardSize(2, 2);
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

  try {
    // 从设置对话框获取参数
    int rows = settingsDialog->getBoardRows();
    int cols = settingsDialog->getBoardCols();
    float boxSize = settingsDialog->getBoardSize();

    qDebug() << "In addPixmap";

    qDebug() << "Board size: " << rows << "x" << cols;
    qDebug() << "Box size: " << boxSize;

    // 设置棋盘格参数
    imageProcessor.setBoardSize(rows, cols);

    calculator.setRows(rows);
    calculator.setCols(cols);
    calculator.setBoxSize(boxSize);

    // 处理图像并检测角点
    QPixmap processedImage =
        imageProcessor.preprocess(img.toImage(), img.size());

    if (!processedImage.isNull()) {
      // 创建缩略图显示
      MyLabel *thumbnailLabel = new MyLabel(processedImage, ui->listWidget);
      connect(thumbnailLabel, &MyLabel::sig_switch_img, this,
              [this](const QPixmap &img) { ui->label->setPixmap(img); });

      // 添加到列表
      QListWidgetItem *item = new QListWidgetItem();
      item->setSizeHint(thumbnailLabel->sizeHint());
      ui->listWidget->addItem(item);
      ui->listWidget->setItemWidget(item, thumbnailLabel);

      // 生成世界坐标（仅在检测到角点时）
      auto imagePoints = imageProcessor.getImagePoints();
      if (!imagePoints.empty()) {
        allImagePoints.push_back(imagePoints.back());
        auto worldPoints =
            calculator.generateWorldCoordinates(imagePoints.back());
        allObjectPoints.push_back(worldPoints); // 同步添加世界坐标点
      }

      qDebug() << "Image processed and added successfully";
    } else {
      qDebug() << "Failed to process image - no corners detected";
    }
  } catch (const std::exception &e) {
    qDebug() << "Error processing image:" << e.what();
  }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
  // QPixmap pixmap = ui->label->pixmap();
  // auto hashValue = calibration.customPixmapHash(pixmap);
  // // 打印哈希值
  // qDebug() << "匹配图像Pixmap Hash: " << hashValue;
  // // 获取当前图像的特征点
  // std::vector<cv::Point2f> featurePoints =
  // calibration.getFeaturePoints(pixmap);

  // if (event->button() == Qt::LeftButton) {
  //   // 获取相对于 ui->label 的坐标
  //   QPoint labelPos = ui->label->mapFromGlobal(event->globalPos());

  //   // 计算缩放比例
  //   qreal scaleX = (qreal)ui->label->width() / (qreal)pixmap.width();
  //   qreal scaleY = (qreal)ui->label->height() / (qreal)pixmap.height();

  //   // 将点击位置从 label 坐标系转换为图像的像素坐标
  //   QPoint imagePos = QPoint(labelPos.x() / scaleX, labelPos.y() / scaleY);

  //   // 设置一个容忍度范围 (根据需要调整此值)
  //   const int tolerance = 10; // 10 pixels tolerance

  //   // 遍历特征点，判断点击位置是否在特征点附近
  //   for (const auto &pt : featurePoints) {
  //     // 将特征点从 cv::Point2f 转换为 QPoint
  //     QPoint featurePoint(pt.x, pt.y);

  //     // 判断点击位置是否与特征点位置接近
  //     if ((imagePos - featurePoint).manhattanLength() <= tolerance) {
  //       qDebug() << "Mouse clicked near a feature point at: " <<
  //       featurePoint;
  //       // 处理点击事件
  //       // 例如，你可以标记该特征点，或者执行其他操作
  //       break; // 如果找到一个特征点就退出
  //     }
  //   }
  // } else {
  //   qDebug() << "No feature points found for this image.";
  // }
}

/**
 * @brief MainWindow::on_actionAdd_Images_triggered
 * add images
 */

void MainWindow::on_actionAdd_Images_triggered() {
  // 创建一个 QMenu 对象，作为下拉菜单
  QMenu *menu = new QMenu(this);

  // 添加两个选项到下拉菜单
  QAction *imageAction = new QAction("Image File", this);
  QAction *videoAction = new QAction("Video File", this);

  // 将选项添加到菜单
  menu->addAction(imageAction);
  menu->addAction(videoAction);

  // 连接信号到槽函数
  connect(imageAction, &QAction::triggered, this, &MainWindow::loadImageFiles);
  connect(videoAction, &QAction::triggered, this, &MainWindow::loadVideoFiles);

  // 将菜单绑定到按钮
  menu->exec(QCursor::pos()); // 在鼠标位置显示菜单
}

void MainWindow::loadImageFiles() {
  QStringList selectedFiles =
      QFileDialog::getOpenFileNames(nullptr, "Open Files", QDir::homePath());

  // 强制弹出对话框
  if (settingsDialog) {
    settingsDialog->exec();
  }

  // 从设置对话框获取参数
  int rows = settingsDialog->getBoardRows();
  int cols = settingsDialog->getBoardCols();
  float boxSize = settingsDialog->getBoardSize();

  qDebug() << "In loadImageFiles";

  qDebug() << "Board size: " << rows << "x" << cols;
  qDebug() << "Box size: " << boxSize;

  foreach (const QString &selectedFile, selectedFiles) {
    qDebug() << selectedFile;
    QPixmap pixmap(selectedFile);
    // 捏吗的缩放，影响这么大
    //  pixmap = pixmap.scaled(ui->label->size(), Qt::KeepAspectRatio,
    //                         Qt::SmoothTransformation);
    addPixmap(pixmap);
  }

  ui->actionCalibrate->setEnabled(true);
  ui->actionSettings->setEnabled(true);
}

void MainWindow::loadVideoFiles() {
  QStringList selectedFiles = QFileDialog::getOpenFileNames(
      nullptr, "Open Video Files", QDir::homePath(),
      "Videos (*.mp4 *.avi *.mkv)");

  foreach (const QString &selectedFile, selectedFiles) {
    qDebug() << "Selected video file:" << selectedFile;
    // 在这里添加加载或处理视频的逻辑
    processVideoFile(selectedFile); // 假设你有一个处理视频的方法
  }
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
  int rows = settingsDialog->getBoardRows();
  int cols = settingsDialog->getBoardCols();
  float boxSize = settingsDialog->getBoardSize();

  // 获取图像尺寸
  QSize size = imageProcessor.getOriginalImageSize();
  cv::Size imageSize(size.width(), size.height());

  qDebug() << "Calibration params - Rows:" << rows << "Cols:" << cols
           << "Box size:" << boxSize;

  QString selectedCameraType =
      settingsDialog ? settingsDialog->getCameraModel() : QString();
  cameraType = selectedCameraType.isEmpty() ? "Standard" : selectedCameraType;
  qDebug() << "Selected camera type:" << cameraType;

  allImagePoints = imageProcessor.getImagePoints();
  allObjectPoints = calculator.getWorldCoordinates();

  qDebug() << "Image points size:" << allImagePoints.size();
  qDebug() << "Object points size:" << allObjectPoints.size();

  if (!allImagePoints.empty()) {
    qDebug() << "Points per image:" << allImagePoints[0].size();
  }

  if (!allObjectPoints.empty()) {
    qDebug() << "Points per object:" << allObjectPoints[0].size();
    qDebug() << "First object point:" << allObjectPoints[0][0].x
             << allObjectPoints[0][0].y << allObjectPoints[0][0].z;
  }

  if (calibration.calibrate(allImagePoints, allObjectPoints, cameraType,
                            imageSize)) {
    cv::Mat cameraMatrix = calibration.getCameraMatrix();
    cv::Mat distCoeffs = calibration.getDistCoeffs();
    double reprojectionError = calibration.getReprojectionError();

    qDebug() << "Calibration successful:";

    qDebug() << "Reprojection error:" << reprojectionError;

    // 新增：打印相机内参矩阵
    qDebug() << "Camera Matrix:";
    for (int r = 0; r < cameraMatrix.rows; ++r) {
      QString rowValues;
      for (int c = 0; c < cameraMatrix.cols; ++c) {
        rowValues +=
            QString::number(cameraMatrix.at<double>(r, c), 'f', 6) + " ";
      }
      qDebug() << rowValues;
    }

    CalibrationResultViewer viewer(cameraMatrix, distCoeffs, reprojectionError,
                                   this);
    viewer.exec();
  } else {
    QMessageBox::warning(
        this, tr("标定失败"),
        tr("相机标定失败，请检查数据是否完整或相机类型是否正确。"));
  }
}

// 视频流标定
void MainWindow::processVideoFile(const QString &selectedFile) {
  cv::VideoCapture cap(selectedFile.toStdString());
  if (!cap.isOpened()) {
    qDebug() << "Error opening video stream or file";
    return;
  }

  int frameSkip = 30; // 每隔30帧抓取一次
  int frameCount = 0;

  cv::Mat frame;

  while (cap.read(frame)) {
    if (++frameCount % frameSkip == 0) {
      // 将 OpenCV 的 Mat 转换为 QImage
      cv::Mat rgbFrame;
      cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB); // 确保颜色通道顺序正确

      QImage qImg((const unsigned char *)(rgbFrame.data), rgbFrame.cols,
                  rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);
      qImg = qImg.copy(); // 创建深拷贝以避免数据被释放的问题

      // 将 QImage 转换为 QPixmap 并添加到 UI 中
      QPixmap pixmap = QPixmap::fromImage(qImg);
      pixmap = pixmap.scaled(ui->label->size(), Qt::KeepAspectRatio,
                             Qt::SmoothTransformation);
      addPixmap(pixmap);

      qDebug() << "Captured frame at position"
               << cap.get(cv::CAP_PROP_POS_FRAMES);
    }
  }
}
