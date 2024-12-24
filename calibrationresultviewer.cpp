// CalibrationResultViewer.cpp

#include "calibrationresultviewer.h"
#include <QHeaderView>
#include <QString>
#include <QVBoxLayout>

CalibrationResultViewer::CalibrationResultViewer(const cv::Mat &cameraMatrix,
                                                 const cv::Mat &distCoeffs,
                                                 double reprojectionError,
                                                 QWidget *parent)
    : QDialog(parent), cameraMatrixTable(new QTableWidget(this)),
      distCoeffsTable(new QTableWidget(this)),
      errorTextEdit(new QTextEdit(this)) {
  setWindowTitle("标定结果");
  setMinimumSize(600, 400);

  QVBoxLayout *layout = new QVBoxLayout(this);

  // 设置相机内参矩阵表格
  cameraMatrixTable->setRowCount(cameraMatrix.rows);
  cameraMatrixTable->setColumnCount(cameraMatrix.cols);
  QStringList cameraHeaders;
  for (int i = 1; i <= cameraMatrix.cols; ++i) {
    cameraHeaders << QString::number(i);
  }
  QStringList cameraRows;
  for (int i = 1; i <= cameraMatrix.rows; ++i) {
    cameraRows << QString::number(i);
  }
  cameraMatrixTable->setHorizontalHeaderLabels(cameraHeaders);
  cameraMatrixTable->setVerticalHeaderLabels(cameraRows);
  cameraMatrixTable->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);
  cameraMatrixTable->verticalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);
  layout->addWidget(new QTextEdit("<h2>相机内参矩阵：</h2>", this)); // 标题
  layout->addWidget(cameraMatrixTable);

  // 设置畸变系数表格
  distCoeffsTable->setRowCount(distCoeffs.rows);
  distCoeffsTable->setColumnCount(distCoeffs.cols);
  QStringList distHeaders;
  for (int i = 1; i <= distCoeffs.cols; ++i) {
    distHeaders << QString::number(i);
  }
  distCoeffsTable->setHorizontalHeaderLabels(distHeaders);
  distCoeffsTable->setVerticalHeaderLabels(QStringList() << "1");
  distCoeffsTable->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);
  distCoeffsTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  layout->addWidget(new QTextEdit("<h2>畸变系数：</h2>", this)); // 标题
  layout->addWidget(distCoeffsTable);

  // 设置重投影误差
  errorTextEdit->setReadOnly(true);
  QString errorText =
      QString("<h2>平均重投影误差：</h2><p>%1</p>").arg(reprojectionError);
  errorTextEdit->setHtml(errorText);
  layout->addWidget(errorTextEdit);

  setLayout(layout);

  // 填充表格数据
  populateTable(cameraMatrixTable, cameraMatrix);
  populateTable(distCoeffsTable, distCoeffs);
}

CalibrationResultViewer::~CalibrationResultViewer() {}

void CalibrationResultViewer::populateTable(QTableWidget *table,
                                            const cv::Mat &mat) {
  // 确保mat是双精度或单精度
  if (mat.type() != CV_32F && mat.type() != CV_64F) {
    qDebug() << "Unsupported matrix type!";
    return;
  }

  for (int i = 0; i < mat.rows; ++i) {
    for (int j = 0; j < mat.cols; ++j) {
      QString cellText;
      if (mat.type() == CV_32F) {
        cellText = QString::number(mat.at<float>(i, j), 'f', 6);
      } else if (mat.type() == CV_64F) {
        cellText = QString::number(mat.at<double>(i, j), 'f', 6);
      }
      table->setItem(i, j, new QTableWidgetItem(cellText));
    }
  }
}