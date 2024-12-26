#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>
#include <QPixmap>
#include <QString>
#include <opencv2/core/types.hpp>
#include <vector>

class ImageProcessor {
public:
  ImageProcessor();
  ~ImageProcessor();

  // 设置棋盘格尺寸
  void setBoardSize(int rows, int cols);

  // 修改预处理方法签名
  QPixmap preprocess(const QImage &image, const QSize &targetSize);

  // 保留文件路径版本用于视频处理
  QPixmap preprocessFromFile(const QString &filePath, const QSize &targetSize);

  // 专门用于标定的图像处理
  bool processForCalibration(const QString &filePath);

  // 获取检测到的角点
  std::vector<std::vector<cv::Point2f>> getImagePoints() const;

  // 清除已存储的角点数据
  void clearImagePoints();

  // 获取原始图像尺寸
  QSize getOriginalImageSize() const;

private:
  QImage convertToGrayscale(const QImage &image);
  QImage reduceNoise(const QImage &image);
  QPixmap detectAndDrawChessboardCorners(const QImage &image, bool drawCorners);

  QImage MatToQImage(const cv::Mat &mat);

  cv::Mat QImageToMat(const QImage &image);

  int m_boardRows;                                     // 棋盘格的行数
  int m_boardCols;                                     // 棋盘格的列数
  QSize m_originalSize;                                // 原始图像尺寸
  QImage m_originalGrayImage;                          // 原始灰度图像
  std::vector<std::vector<cv::Point2f>> m_imagePoints; // 存储所有图像的角点
};

#endif // IMAGEPROCESSOR_H