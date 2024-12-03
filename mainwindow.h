#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private:
  void addPixmap(const QPixmap &);

private slots:
  void on_actionAdd_Images_triggered();
  void on_actionSettings_triggered();
  void on_actionCalilbrate_triggered();

private:
  Ui::MainWindow *ui;

  // settings
  QString cameraModel;
  int checkerBoardLength;
  int checkerBoardWidth;
  int squareSize;

};
#endif // MAINWINDOW_H
