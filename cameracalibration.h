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
                 const QString &cameraType, const cv::Size &imageSize);
  // 获取标定结果
  cv::Mat getCameraMatrix() const { return cameraMatrix; }
  cv::Mat getDistCoeffs() const { return distCoeffs; }
  double getReprojectionError() const { return totalAvgErr; }
  bool isCalibrated() const { return !cameraMatrix.empty(); }
  const std::vector<double> &getPerViewErrors() const { return perViewErrors; }

private:
  // 标定结果
  cv::Mat cameraMatrix;
  cv::Mat distCoeffs;
  std::vector<cv::Mat> rvecs;
  std::vector<cv::Mat> tvecs;
  double totalAvgErr;
  std::vector<double> perViewErrors; // 存储每张图片的重投影误差

  // 计算重投影误差
  double computeReprojectionErrors(
      const std::vector<std::vector<cv::Point2f>> &imagePoints,
      const std::vector<std::vector<cv::Point3f>> &objectPoints,
      const QString &cameraType);

  void clear() {
    cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
    distCoeffs.release(); // 将在标定时根据类型初始化
    rvecs.clear();
    tvecs.clear();
    totalAvgErr = 0.0;
  }
};

#endif // CAMERACALIBRATION_H
