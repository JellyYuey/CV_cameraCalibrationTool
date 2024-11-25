#include "mainwindow.h"
#include "mylabel.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <opencv2/opencv.hpp> // 引入 OpenCV 头文件

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

void MainWindow::on_actionCalilbrate_triggered() {
  // Calibration start
  // 示例：使用 OpenCV 读取并显示图像
  cv::Mat image = cv::imread("path_to_image.jpg", cv::IMREAD_COLOR);
  if (image.empty()) {
    qDebug() << "Failed to load image.";
    return;
  }
  cv::Mat gray;
  cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
  cv::imwrite("gray_image.jpg", gray);
  qDebug() << "Image converted to grayscale and saved as gray_image.jpg.";
}