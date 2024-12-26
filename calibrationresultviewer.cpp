// CalibrationResultViewer.cpp

#include "calibrationresultviewer.h"
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>

// 修改 Charts 头文件引用方式
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QLineSeries> // 添加这行
#include <QValueAxis>

CalibrationResultViewer::CalibrationResultViewer(const cv::Mat &cameraMatrix,
                                                 const cv::Mat &distCoeffs,
                                                 double reprojectionError,
                                                 QWidget *parent)
    : QDialog(parent), cameraMatrixTable(new QTableWidget(this)),
      distCoeffsTable(new QTableWidget(this)),
      errorTextEdit(new QTextEdit(this)) {
  setWindowTitle("标定结果");
  setMinimumSize(600, 400);

  // 将主布局改用水平布局
  QHBoxLayout *mainLayout = new QHBoxLayout(this);
  QWidget *leftWidget = new QWidget(this);
  QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);

  // 新增：把相机内参表格放进 QGroupBox
  QGroupBox *cameraBox = new QGroupBox(tr("相机内参矩阵"), this);
  QVBoxLayout *cameraBoxLayout = new QVBoxLayout(cameraBox);
  cameraBoxLayout->addWidget(cameraMatrixTable);
  leftLayout->addWidget(cameraBox);

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

  // 畸变系数表格也放进 QGroupBox
  QGroupBox *distBox = new QGroupBox(tr("畸变系数"), this);
  QVBoxLayout *distBoxLayout = new QVBoxLayout(distBox);
  distBoxLayout->addWidget(distCoeffsTable);
  leftLayout->addWidget(distBox);

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

  // 重投影误差改用 QLabel 显示
  QLabel *errorLabel = new QLabel(
      QString(tr("平均重投影误差: %1")).arg(reprojectionError), this);
  leftLayout->addWidget(errorLabel);

  leftWidget->setLayout(leftLayout);
  mainLayout->addWidget(leftWidget);

  // 右侧图表区域
  QChart *chart = new QChart();

  // 创建柱状图
  QBarSet *barSet = new QBarSet(tr("畸变系数"));
  int totalPoints = 0;
  for (int r = 0; r < distCoeffs.rows; ++r) {
    for (int c = 0; c < distCoeffs.cols; ++c) {
      double val = (distCoeffs.type() == CV_32F) ? distCoeffs.at<float>(r, c)
                                                 : distCoeffs.at<double>(r, c);
      barSet->append(val);
      totalPoints++;
    }
  }

  QBarSeries *barSeries = new QBarSeries(this);
  barSeries->append(barSet);
  chart->addSeries(barSeries);

  // 添加重投影误差线
  QLineSeries *errorLine = new QLineSeries(this);
  errorLine->setName(tr("重投影误差"));

  // 添加足够的点以确保线条覆盖整个图表
  errorLine->append(0, reprojectionError);
  errorLine->append(totalPoints - 1, reprojectionError);

  // 设置线条样式
  QPen pen(Qt::red);
  pen.setWidth(2);
  pen.setStyle(Qt::DashLine);
  errorLine->setPen(pen);

  chart->addSeries(errorLine);

  // 创建并设置坐标轴
  chart->createDefaultAxes();

  // 获取并配置Y轴
  QValueAxis *axisY =
      qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).first());
  if (axisY) {
    // 计算显示范围
    double minY = std::min(barSet->at(0), reprojectionError);
    double maxY = std::max(barSet->at(0), reprojectionError);
    for (int i = 1; i < barSet->count(); ++i) {
      minY = std::min(minY, barSet->at(i));
      maxY = std::max(maxY, barSet->at(i));
    }

    // 增加更大的边距确保线条可见
    double margin = 0.2 * std::max(std::abs(minY), std::abs(maxY));
    axisY->setRange(minY - margin, maxY + margin);
    axisY->applyNiceNumbers();
  }

  // 设置图表标题和图例
  chart->setTitle(tr("畸变系数与重投影误差"));
  chart->legend()->show();
  chart->legend()->setAlignment(Qt::AlignBottom);

  QChartView *chartView = new QChartView(chart, this);
  chartView->setRenderHint(QPainter::Antialiasing);
  mainLayout->addWidget(chartView);

  setLayout(mainLayout);

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