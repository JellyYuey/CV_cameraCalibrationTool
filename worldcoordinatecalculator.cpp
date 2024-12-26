#include "worldcoordinatecalculator.h"
#include <QDebug>

// Generate world coordinates for the chessboard

std::vector<cv::Point3f> WorldCoordinateCalculator::generateWorldCoordinates(
    const std::vector<cv::Point2f> &imagePoints) {
  std::vector<cv::Point3f> worldPoints;
  qDebug() << "In generateWorldCoordinates, Rows:" << rows << "Cols:" << cols
           << "Box size:" << boxSize;
  for (int i = 0; i < rows - 1; ++i) {
    for (int j = 0; j < cols - 1; ++j) {
      worldPoints.push_back(cv::Point3f(j * boxSize, i * boxSize, 0.0f));
    }
  }
  worldCoordinates.push_back(worldPoints);
  return worldPoints;
}
