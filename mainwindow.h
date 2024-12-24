#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cameracalibration.h"
#include "settings.h"
#include "worldcoordinatecalculator.h"
#include <QMainWindow>
#include <QPointer>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void onImageClicked(QPoint point);
  void onImageDragged(QPoint point);
  void mousePressEvent(QMouseEvent *event) override;
  void processVideoFile(const QString &selectedFilee);

private:
  void addPixmap(const QPixmap &);

private slots:
  void on_actionAdd_Images_triggered();
  void loadImageFiles();
  void loadVideoFiles();
  void on_actionSettings_triggered();

  void on_actionCalibrate_triggered();

private:
  Ui::MainWindow *ui;
  QString cameraType;
  QString calibrationBoardType;
  int calibrationBoardSize;
  QSettings settings;
  QPointer<Settings> settingsDialog;
  CameraCalibration calibration;
  WorldCoordinateCalculator calculator;

  std::vector<std::vector<cv::Point2f>> allImagePoints; // 所有图像的像素坐标
  std::vector<std::vector<cv::Point3f>> allObjectPoints; // 所有图像的世界坐标
};
#endif // MAINWINDOW_H
