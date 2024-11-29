#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QPointer>
#include <settings.h>
#include "CameraCalibration.h"
#include "worldcoordinatecalculator.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void onImageClicked(QPoint point);
    void onImageDragged(QPoint point);
    void mousePressEvent(QMouseEvent *event) override;

private:
    void addPixmap(const QPixmap&);

private slots:
    void on_actionAdd_Images_triggered();
    void on_actionSettings_triggered();

    void on_actionCalilbrate_triggered();
    // void on_label_click(QMouseEvent *event);

private:
    Ui::MainWindow *ui;
    QString cameraType;
    QString calibrationBoardType;
    int calibrationBoardSize;
    QSettings settings;
    QPointer<Settings> settingsDialog;
    CameraCalibration calibration;
    WorldCoordinateCalculator calculator;
    std::vector<cv::Point2f> featurePoints;  // 存储检测到的特征点


};
#endif // MAINWINDOW_H
