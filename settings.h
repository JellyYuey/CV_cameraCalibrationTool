#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class Settings;
}

class Settings : public QDialog {
  Q_OBJECT

public:
  explicit Settings(QWidget *parent = nullptr);
  ~Settings();
  float getBoardSize();

  QString getCameraModel() const; // 添加此方法

  int getBoardRows() const;

  int getBoardCols() const;

private slots:
  void saveSettings();
  void loadSettings();
  void on_okButton_clicked();
  void on_cancelButton_clicked();

private:
  Ui::Settings *ui;
};

#endif // SETTINGS_H
