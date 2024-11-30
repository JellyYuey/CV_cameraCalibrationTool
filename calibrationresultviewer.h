// CalibrationResultViewer.h

#ifndef CALIBRATIONRESULTVIEWER_H
#define CALIBRATIONRESULTVIEWER_H

#include <QDialog>
#include <QTableWidget>
#include <QTextEdit>
#include <opencv2/opencv.hpp>

class CalibrationResultViewer : public QDialog {
  Q_OBJECT

public:
  explicit CalibrationResultViewer(const cv::Mat &cameraMatrix,
                                   const cv::Mat &distCoeffs,
                                   double reprojectionError,
                                   QWidget *parent = nullptr);
  ~CalibrationResultViewer();

private:
  QTableWidget *cameraMatrixTable;
  QTableWidget *distCoeffsTable;
  QTextEdit *errorTextEdit;

  void displayResults(const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs,
                      double reprojectionError);
  void populateTable(QTableWidget *table, const cv::Mat &mat);
};

#endif // CALIBRATIONRESULTVIEWER_H