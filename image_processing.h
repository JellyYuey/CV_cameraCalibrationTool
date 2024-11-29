#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <opencv2/opencv.hpp>
#include <vector>

class ImageProcessing {
public:
  ImageProcessing();
  ~ImageProcessing();

  // 接口：处理图像，提取角点
  bool processImages(const std::vector<cv::Mat> &images,
                     std::vector<std::vector<cv::Point2f>> &imagePoints);

  // 接口：显示提取的角点（可选）
  void visualizeCorners(const cv::Mat &image,
                        const std::vector<cv::Point2f> &corners);

  // 接口：优化角点
  void optimizeCorners(cv::Mat &image, std::vector<cv::Point2f> &corners);

  // 接口：获取世界坐标系中的点
  void getWorldCoordinates(std::vector<cv::Point3f> &worldPoints);

private:
  int boardWidth = 9;       // 棋盘格宽度
  int boardHeight = 6;      // 棋盘格高度
  float squareSize = 30.0f; // 每个小方块的大小（单位：mm）
};

#endif // IMAGEPROCESSING_H
