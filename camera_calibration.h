#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

#include <QString>
#include <opencv2/opencv.hpp>
#include <vector>

class CameraCalibration {
public:
  CameraCalibration();
  ~CameraCalibration();

  // 设置标定所需的相机参数
  void setCameraParameters(const QString &cameraType, int boardWidth,
                           int boardHeight, float squareSize);

  // 执行相机标定
  bool calibrate(const std::vector<cv::Mat> &images,
                 const std::vector<std::vector<cv::Point2f>> &imagePoints,
                 std::vector<cv::Point3f> &worldPoints);

  // 获取标定结果
  cv::Mat getCameraMatrix() const;
  cv::Mat getDistortionCoefficients() const;

  // 计算重投影误差
  double computeReprojectionError(
      const std::vector<std::vector<cv::Point3f>> &objectPoints,
      const std::vector<std::vector<cv::Point2f>> &imagePoints,
      const std::vector<cv::Mat> &rvecs,
      const std::vector<cv::Mat> &tvecs) const;

private:
  cv::Mat cameraMatrix;
  cv::Mat distortionCoefficients;
  cv::Size imageSize; // 图像尺寸
  QString cameraType;
  int boardWidth;
  int boardHeight;
  float squareSize; // 棋盘格每个格子的实际尺寸（单位：mm）
};

#endif // CAMERACALIBRATION_H
