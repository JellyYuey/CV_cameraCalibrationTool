#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

#include <opencv2/opencv.hpp>
#include <vector>

class CameraCalibration {
public:
  CameraCalibration();
  ~CameraCalibration();

  // 设置棋盘格大小和每个格子的世界坐标大小
  void setBoardSize(int boardWidth, int boardHeight, float squareSize);

  // 执行相机标定
  bool calibrate(const std::vector<cv::Mat> &images,
                 const std::vector<std::vector<cv::Point2f>> &imagePoints);

  // 计算重投影误差
  double computeReprojectionError(
      const std::vector<cv::Mat> &images,
      const std::vector<std::vector<cv::Point2f>> &imagePoints);

  // 获取标定结果
  cv::Mat getCameraMatrix() const;
  cv::Mat getDistCoeffs() const;
  cv::Mat getRotationVectors() const;
  cv::Mat getTranslationVectors() const;

private:
  cv::Mat cameraMatrix;
  cv::Mat distCoeffs;
  std::vector<cv::Mat> rvecs, tvecs; // 旋转向量和平移向量

  int boardWidth;
  int boardHeight;
  float squareSize;
};

#endif // CAMERACALIBRATION_H
