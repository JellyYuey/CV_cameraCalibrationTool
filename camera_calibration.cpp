#include "camera_calibration.h"
#include <opencv2/opencv.hpp>

CameraCalibration::CameraCalibration()
    : boardWidth(9), boardHeight(6), squareSize(0.025f) {}

CameraCalibration::~CameraCalibration() {}

void CameraCalibration::setBoardSize(int boardWidth, int boardHeight,
                                     float squareSize) {
  this->boardWidth = boardWidth;
  this->boardHeight = boardHeight;
  this->squareSize = squareSize;
}

bool CameraCalibration::calibrate(
    const std::vector<cv::Mat> &images,
    const std::vector<std::vector<cv::Point2f>> &imagePoints) {
  std::vector<std::vector<cv::Point3f>> objectPoints;
  std::vector<cv::Point3f> objectCorners;

  // 生成棋盘格的世界坐标点
  for (int i = 0; i < boardHeight; i++) {
    for (int j = 0; j < boardWidth; j++) {
      objectCorners.push_back(cv::Point3f(j * squareSize, i * squareSize, 0));
    }
  }

  for (size_t i = 0; i < images.size(); i++) {
    objectPoints.push_back(objectCorners);
  }

  // 执行相机标定
  std::vector<cv::Mat> rvecs, tvecs;
  double rms = cv::calibrateCamera(objectPoints, imagePoints, images[0].size(),
                                   cameraMatrix, distCoeffs, rvecs, tvecs);
  return rms < 1.0; // 简单的误差阈值判断
}

cv::Mat CameraCalibration::getCameraMatrix() const { return cameraMatrix; }

cv::Mat CameraCalibration::getDistCoeffs() const { return distCoeffs; }
