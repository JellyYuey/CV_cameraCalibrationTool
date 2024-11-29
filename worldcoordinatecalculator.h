#ifndef WORLDCOORDINATECALCULATOR_H
#define WORLDCOORDINATECALCULATOR_H
#include <opencv2/opencv.hpp>
#include <vector>

class WorldCoordinateCalculator
{
public:
    // Constructor initializing the number of rows, columns, and the physical size of each square
    WorldCoordinateCalculator(int rows, int cols, float boxSize):rows(rows), cols(cols), boxSize(boxSize) {}

    // Generate world coordinates based on the given image points
    std::vector<cv::Point3f> generateWorldCoordinates(const std::vector<cv::Point2f>& imagePoints);

    // Get the world coordinates (can be used after generation)
    std::vector<cv::Point3f> getWorldCoordinates() {return worldCoordinates;}

private:
    int rows;         // Number of rows in the chessboard
    int cols;         // Number of columns in the chessboard
    float boxSize;    // Physical size of each square on the chessboard
    std::vector<cv::Point3f> worldCoordinates; // Stores the calculated world coordinates
};

#endif // WORLDCOORDINATECALCULATOR_H
