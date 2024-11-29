#ifndef WORLDCOORDINATECALCULATOR_H
#define WORLDCOORDINATECALCULATOR_H
#include <opencv2/opencv.hpp>
#include <vector>

class WorldCoordinateCalculator
{
public:
    // Constructor initializing the number of rows, columns, and the physical size of each square
<<<<<<< HEAD
    WorldCoordinateCalculator(){};
=======
>>>>>>> 10cf5d6e155af2e847060c1af42f091a5c3ddb82
    WorldCoordinateCalculator(int rows, int cols, float boxSize):rows(rows), cols(cols), boxSize(boxSize) {}

    // Generate world coordinates based on the given image points
    std::vector<cv::Point3f> generateWorldCoordinates(const std::vector<cv::Point2f>& imagePoints);

    // Get the world coordinates (can be used after generation)
    std::vector<std::vector<cv::Point3f>> getWorldCoordinates() {return worldCoordinates;}

    void setRows(int rows){this->rows=rows;}
    void setCols(int cols){this->cols=cols;}
    void setBoxSize(float boxSize){this->boxSize=boxSize;}


private:
    int rows;         // Number of rows in the chessboard
    int cols;         // Number of columns in the chessboard
    float boxSize;    // Physical size of each square on the chessboard
    std::vector<std::vector<cv::Point3f>> worldCoordinates;
};

#endif // WORLDCOORDINATECALCULATOR_H
