#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settings.h"
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

  // 新增的参数获取函数
  QString getCameraType() const;
  QString getCalibrationBoardType() const;
  int getCalibrationBoardSize() const;

private:
  void addPixmap(const QPixmap &);

private slots:
  void on_actionAdd_Images_triggered();
  void on_actionSettings_triggered();

  void on_actionCalibrate_triggered();

private:
  Ui::MainWindow *ui;
  QString cameraType;
  QString calibrationBoardType;
  int calibrationBoardSize;
  QSettings settings;
  QPointer<Settings> settingsDialog;
};
#endif // MAINWINDOW_H