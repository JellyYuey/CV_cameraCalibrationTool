#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <opencv2/opencv.hpp>
#include <vector>

class ImageProcessing {
public:
  // 构造函数和析构函数
  ImageProcessing();
  ~ImageProcessing();

  // 接口：处理图像，提取角点
  bool processImages(const std::vector<cv::Mat> &images,
                     std::vector<std::vector<cv::Point2f>> &imagePoints);

  // 接口：显示提取的角点
  void visualizeCorners(const cv::Mat &image,
                        const std::vector<cv::Point2f> &corners);

  // 接口：获取图像中的世界坐标系点
  void getWorldCoordinates(std::vector<cv::Point3f> &worldPoints);

  // 可选：接口，用于优化角点（如果需要）
  void optimizeCorners(cv::Mat &image, std::vector<cv::Point2f> &corners);
};

#endif // IMAGEPROCESSING_H
