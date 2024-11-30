#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

#include <QImage>
#include <QMouseEvent>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <vector>

class CameraCalibration {
public:
  CameraCalibration();
  ~CameraCalibration();
  cv::Mat getProcessedImage();

  bool processImage(const QImage &inputImage,
                    std::vector<cv::Point2f> &corners);
  std::vector<cv::Point2f> getFeaturePoints(const QPixmap &pixmap);
  void setFeaturePoints(const QPixmap &pixmap,
                        const std::vector<cv::Point2f> &corners);
  size_t customPixmapHash(const QPixmap &pixmap);
  // 正确的定义方式
  QHash<size_t, std::vector<cv::Point2f>> imageFeaturePoints;

  static QImage MatToQImage(const cv::Mat &mat);

  static cv::Mat QImageToMat(const QImage &image);

  /**
   * @brief 进行相机标定
   *
   * @param imagePoints 所有图像的像素坐标
   * @param objectPoints 所有图像的世界坐标
   * @param cameraType 相机类型（Standard 或 Fisheye）
   * @return true 标定成功
   * @return false 标定失败
   */
  bool calibrate(const std::vector<std::vector<cv::Point2f>> &imagePoints,
                 const std::vector<std::vector<cv::Point3f>> &objectPoints,
                 const QString &cameraType);
  // 获取标定结果
  cv::Mat getCameraMatrix() const { return cameraMatrix; }
  cv::Mat getDistCoeffs() const { return distCoeffs; }
  double getReprojectionError() const { return totalAvgErr; }

private:
  cv::Size imageSize;   // 图像尺寸
  cv::Mat processedMat; // The processed image with corners drawn
  std::vector<std::vector<cv::Point2f>>
      allFeaturePoints; // 保存所有图片的特征点

  // 标定结果
  cv::Mat cameraMatrix;
  cv::Mat distCoeffs;
  std::vector<cv::Mat> rvecs;
  std::vector<cv::Mat> tvecs;
  double totalAvgErr;

  // 计算重投影误差
  double computeReprojectionErrors(
      const std::vector<std::vector<cv::Point2f>> &imagePoints,
      const std::vector<std::vector<cv::Point3f>> &objectPoints);

  bool detectCorners(const cv::Mat &image, std::vector<cv::Point2f> &corners);

  void optimizeCorners(cv::Mat &image, std::vector<cv::Point2f> &corners);
};

#endif // CAMERACALIBRATION_H
