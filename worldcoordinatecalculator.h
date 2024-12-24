#ifndef WORLDCOORDINATECALCULATOR_H
#define WORLDCOORDINATECALCULATOR_H
#include <opencv2/opencv.hpp>
#include <vector>

class WorldCoordinateCalculator {
public:
  // Constructor initializing the number of rows, columns, and the physical size
  // of each square

  WorldCoordinateCalculator() {};
  WorldCoordinateCalculator(int rows, int cols, float boxSize)
      : rows(rows), cols(cols), boxSize(boxSize) {}

  // Generate world coordinates based on the given image points
  std::vector<cv::Point3f>
  generateWorldCoordinates(const std::vector<cv::Point2f> &imagePoints);

  // Get the world coordinates (can be used after generation)
  std::vector<std::vector<cv::Point3f>> getWorldCoordinates() {
    return worldCoordinates;
  }

  void setRows(int rows) { this->rows = rows; }
  void setCols(int cols) { this->cols = cols; }
  void setBoxSize(float boxSize) { this->boxSize = boxSize; }

  // 获取棋盘格参数
  int getRows() const { return rows; }
  int getCols() const { return cols; }
  float getBoxSize() const { return boxSize; }

private:
  int rows;      // Number of rows in the chessboard
  int cols;      // Number of columns in the chessboard
  float boxSize; // Physical size of each square on the chessboard
  std::vector<std::vector<cv::Point3f>> worldCoordinates;
};

#endif // WORLDCOORDINATECALCULATOR_H
